module Rubykon
  class GroupTracker

    attr_reader :groups, :stone_to_group

    def initialize(groups = {}, stone_to_group = {})
      @groups         = groups
      @stone_to_group = stone_to_group
    end

    def assign(identifier, color, board)
      neighbours_by_color = color_to_neighbour(board, identifier)
      join_group_of_friendly_stones(neighbours_by_color[color], identifier)
      create_own_group(identifier) unless group_id_of(identifier)
      add_liberties(neighbours_by_color[Board::EMPTY], identifier)
      take_liberties_of_enemies(neighbours_by_color[Game.other_color(color)], identifier, board, color)
    end

    def liberty_count_at(identifier)
    