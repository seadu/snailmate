# frozen_string_literal: true

module RBS
  module Prototype
    module Helpers
      private

      def block_from_body(node)
        _, args_node, body_node = node.children

        _pre_num, _pre_init, _opt, _first_post, _post_num, _post_init, _rest, _kw, _kwrest, block = args_from_node(args_node)

        method_block = nil

        if block
          method_block = Types::Block.new(
            required: false,
            type: Types::Function.empty(untyped),
            self_type: nil
          )
        end

        if body_node
          if (yields = any_node?(body_node) {|n| n.type == :YIELD })
            method_block = Types::Block.new(
              required: true,
              type: Types::Function.empty(untyped),
              self_type: nil
            )

            yields.each do |yield_node|
              array_content = yield_node.children[0]&.children&.compact || []

              # @type var keywords: node?
              positionals, keywords = if keyword_hash?(array_content.last)
                                        [array_content.take(array_content.size - 1), array_content.last]
                                      else
                                        [array_content, nil]
                                      end

              if (diff = positionals.size - method_block.type.required_positionals.size) > 0
                diff.times do
                  method_block.type.required_positionals << Types::Function::Param.new(
                    type: untyped,
                    name: nil
                  )
                end
              end

              if keywords
                keywords.children[0].children.each_slice(2) do |key_node, value_node|
                  if key_node
                    key = key_node.children[0]
                    method_block.type.required_keywords[key] ||=
                      Types::Function::Param.new(
                        type: untyped,
                        name: nil
                      )
                  end
                end
              end
            end
          end
        end

        method_block
      end

     