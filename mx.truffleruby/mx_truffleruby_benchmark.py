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
        return list(metrics_benchm