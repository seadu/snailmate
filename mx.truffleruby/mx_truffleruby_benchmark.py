# Copyright (c) 2016 Oracle and/or its affiliates. All rights reserved. This
# code is released under a tri EPL/GPL/LGPL license. You can use it,
# redistribute it and/or modify it under the terms of the:
#
# Eclipse Public License version 2.0, or
# GNU General Public License version 2, or
# GNU Lesser General Public License version 2.1.

from __future__ import division

import os
import json
import pipes
import signal
import subprocess
import sys
import time
from os.path import join

import mx
import mx_benchmark

# TODO IMPORTANT: you need RUBY_BENCHMARKS=true for this file to be imported by mx_truffleruby.py

# Utilities

_suite = mx.suite('truffleruby')
rubyDir = _suite.dir

def jt(args, nonZeroIsFatal=True, out=None, err=None, timeout=None, env=None, cwd=None):
    jt = join(rubyDir, 'tool', 'jt.rb')
    return mx.run(['ruby', jt] + args, nonZeroIsFatal=nonZeroIsFatal, out=out, err=err, timeout=timeout, env=env, cwd=cwd)

FNULL = open(os.devnull, 'w')

class BackgroundServerTask:
    def __init__(self, args):
        self.args = args

    def __enter__(self):
        if mx._opts.verbose:
            mx.log(' '.join(['(background)'] + [pipes.quote(arg) for arg in self.args]))
        self.process = subprocess.Popen(self.args, start_new_session=True)
        mx._addSubprocess(self.process, self.args)

    def __exit__(self, exc_type, value, traceback):
        self.process.send_signal(signal.SIGINT)
        self.process.wait()

    def is_running(self):
        return self.process.poll() is None

class BackgroundJT(BackgroundServerTask):
    def __init__(self, args):
        jt = join(rubyDir, 'tool', 'jt.rb')
        BackgroundServerTask.__init__(self, ['ruby', jt] + args)


# Benchmarks

class RubyBenchmarkSuite(mx_benchmark.BenchmarkSuite):
    def group(self):
        return 'Graal'

    def subgroup(self):
        return 'truffleruby'

    def vmArgs(self, bmSuiteArgs):
        return mx_benchmark.splitArgs(bmSuiteArgs, bmSuiteArgs)[0]

    def runArgs(self, bmSuiteArgs):
        return mx_benchmark.splitArgs(bmSuiteArgs, bmSuiteArgs)[1]

    def default_benchmarks(self, bmSuiteArgs):
        return self.benchmarkList(bmSuiteArgs)

    def run(self, benchmarks, bmSuiteArgs):
        def fixUpResult(result):
            result.update({
                'host-vm': os.environ.get('HOST_VM', 'host-vm'),
                'host-vm-config': os.environ.get('HOST_VM_CONFIG', 'host-vm-config'),
                'guest-vm': os.environ.get('GUEST_VM', 'guest-vm'),
                'guest-vm-config': os.environ.get('GUEST_VM_CONFIG', 'guest-vm-config')
            })
            return result

        return [fixUpResult(r) for b in benchmarks or self.default_benchmarks(bmSuiteArgs) for r in self.runBenchmark(b, bmSuiteArgs)]

    def runBenchmark(self, benchmark, bmSuiteArgs):
        raise NotImplementedError()

build_stats_benchmarks = {
    'binary-size': ['size', 'MiB'],
    'build-time': ['time', 's'],
    'runtime-compilable-methods': ['count', '#']
}

class BuildStatsBenchmarkSuite(RubyBenchmarkSuite):
    def benchmarkList(self, bmSuiteArgs):
        return list(build_stats_benchmarks.keys())

    def name(self):
        return 'build-stats'

    def runBenchmark(self, benchmark, bmSuiteArgs):
        out = mx.OutputCapture()

        jt(['build_stats', benchmark, '--json'] + bmSuiteArgs, out=out)

        data = json.loads(out.data)
        (metric_name, metric_unit) = build_stats_benchmarks[benchmark]

        ret = {
            'benchmark': benchmark,
            'metric.name': metric_name,
            'metric.value': data[benchmark],
            'metric.unit': metric_unit,
            'metric.better': 'lower',
        }

        if data[benchmark] == -1:
            ret['error'] = 'failed'

        return [ret]

metrics_benchmarks = {
    'hello': ['-e', "puts 'hello'"],
    'compile-mandelbrot': [join(rubyDir, 'bench', 'metrics', 'mandelbrot.rb')]
}

default_metrics_benchmarks = ['hello', 'compile-mandelbrot']

class MetricsBenchmarkSuite(RubyBenchmarkSuite):
    def benchmarkList(self, bmSuiteArgs):
        return list(metrics_benchmarks.keys())

    def default_benchmarks(self, bmSuiteArgs):
        return default_metrics_benchmarks

class AllocationBenchmarkSuite(MetricsBenchmarkSuite):
    def name(self):
        return 'allocation'

    def runBenchmark(self, benchmark, bmSuiteArgs):
        out = mx.OutputCapture()

        jt(['metrics', 'alloc', '--json'] + metrics_benchmarks[benchmark] + bmSuiteArgs, out=out)

        data = json.loads(out.data)

        return [{
            'benchmark': benchmark,
            'metric.name': 'memory',
            'metric.value': sample,
            'metric.unit': 'B',
            'metric.better': 'lower',
            'metric.iteration': n
        } for n, sample in enumerate(data['samples'])]

class InstructionsBenchmarkSuite(MetricsBenchmarkSuite):
    def name(self):
        return 'instructions'

    def runBenchmark(self, benchmark, bmSuiteArgs):
        out = mx.OutputCapture()

        jt(['metrics', 'instructions', '--json'] + metrics_benchmarks[benchmark] + bmSuiteArgs, out=out)

        data = json.loads(out.data)

        return [{
            'benchmark': benchmark,
            'metric.name': 'count',
            'metric.value': data['instructions'],
            'metric.unit': '#',
            'metric.better': 'lower',
        }]

class MinHeapBenchmarkSuite(MetricsBenchmarkSuite):
    def name(self):
        return 'minheap'

    def runBenchmark(self, benchmark, bmSuiteArgs):
        out = mx.OutputCapture()

        jt(['metrics', 'minheap', '--json'] + metrics_benchmarks[benchmark] + bmSuiteArgs, out=out)

        data = json.loads(out.data)

        return [{
            'benchmark': benchmark,
            'metric.name': 'memory',
            'metric.value': data['min'],
            'metric.unit': 'MiB',
            'metric.better': 'lower'
        }]

class MaxRssBenchmarkSuite(MetricsBenchmarkSuite):
    def name(self):
        return 'maxrss'

    def runBenchmark(self, benchmark, bmSuiteArgs):
        out = mx.OutputCapture()

        jt(['metrics', 'maxrss', '--json'] + metrics_benchmarks[benchmark] + bmSuiteArgs, out=out)

        data = json.loads(out.data)

        return [{
            'benchmark': benchmark,
            'extra.metric.region': region,
            'metric.name': 'memory',
            'metric.value': sample,
            'metric.unit': 'MiB',
            'metric.better': 'lower',
            'metric.iteration': n
        } for region, region_data in data.items() for n, sample in enumerate(region_data['samples'])]

class TimeBenchmarkSuite(MetricsBenchmarkSuite):
    def name(self):
        return 'time'

    def runBenchmark(self, benchmark, bmSuiteArgs):
        out = mx.OutputCapture()

        jt(['metrics', 'time', '--json'] + metrics_benchmarks[benchmark] + bmSuiteArgs, out=out)

        lines = [line for line in out.data.split('\n') if len(line) > 0]
        mx.log('\n'.join(lines[0:-1]))

        json_data = lines[-1]
        mx.log('JSON:')
        mx.log(json_data)
        data = json.loads(json_data)

        return [{
            'benchmark': benchmark,
            'extra.metric.region': region,
            'metric.name': 'time',
            'metric.value': sample,
            'metric.unit': 'ms',
            'metric.better': 'lower',
            'metric.iteration': n
        } for region, region_data in data.items() for n, sample in enumerate(region_data['samples'])]

class AllBenchmarksBenchmarkSuite(RubyBenchmarkSuite):
    def config(self):
        return {'kind': 'simple'}

    def benchmarkList(self, bmSuiteArgs):
        raise NotImplementedError()

    def name(self):
        raise NotImplementedError()

    def time(self):
        raise NotImplementedError()

    def directory(self):
        return self.name()

    def filterLines(self, lines):
        data = []
        for line in lines:
            try:
                data.append(int(line))
            except ValueError:
                try:
                    data.append(float(line))
                except ValueError:
                    mx.log_error(line)
        if len(data) % 3 != 0:
            raise AssertionError("Number of values not a multiple of 3")
        return data

    def runBenchmark(self, benchmark, bmSuiteArgs):
        directory = self.directory()
        if directory is None:
            directory, benchmark = benchmark.split('/')

        arguments = ['benchmark']
        if self.config()['kind'] == 'simple':
            arguments.extend(['--simple', '--iterations', '--elapsed'])
            time = self.time()
            if isinstance(time, dict):
                if benchmark in time:
                    time = str(time[benchmark])
                else:
                    time = str(time['default'])
            else:
                time = str(self.time())
            arguments.extend(['--time', time])
        elif self.config()['kind'] == 'fixed-iterations':
            iterations_config = self.config()['iterations'][benchmark]
            fixed_iterations = sorted(iterations_config.keys())
            fixed_iterations_arg = ','.join([str(i) for i in fixed_iterations])
            arguments.extend(['--iterations', '--elapsed', '--ips'])
            arguments.extend(['--fixed-iterations'])
            arguments.extend([fixed_iterations_arg])
            if iterations_config != {1:'single-shot'}:
                # single-shot benchmarks use subprocesses so startup is already included
                arguments.extend(['--start-time', 'START_TIME_SET_BY_JT_BENCHMARK'])
        else:
            raise AssertionError("Unknown benchmark kind: " + self.config()['kind'])

        if ':' in benchmark:
            benchmark_file, _, benchmark_name = benchmark.partition(':')
            benchmark_names = [benchmark_name]
        else:
            benchmark_file = benchmark
            benchmark_names = []
        arguments.extend(['bench/' + directory + '/' + benchmark_file + '.rb'])
        arguments.extend(benchmark_names)
        arguments.extend(bmSuiteArgs)
        out = mx.OutputCapture()

        if jt(arguments, out=out, nonZeroIsFatal=False) == 0:
            mx.log(out.data)
            lines = out.data.split('\n')[1:-1]

            data = self.filterLines(lines)
            iterations = [d for n, d in enumerate(data) if n % 3 == 0]
            elapsed = [d for n, d in enumerate(data) if n % 3 == 1]
            samples = [d for n, d in enumerate(data) if n % 3 == 2]

            if lines[-1] == 'optimised away':
                return [{
                    'benchmark': benchmark,
                    'metric.name': 'throughput',
                    'metric.value': sample,
                    'me