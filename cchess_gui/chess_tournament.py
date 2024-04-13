from enum import IntEnum
from chess_board import *
from tabulate import tabulate


class BoardResult(IntEnum):
    RED_WIN = 0
    BLACK_WIN = 1
    NOT_END = 2

class ChessTournament:
    def __init__(self, players):
        self.players = players
        self.scores = {}
        self.results = [[0] * (len(players)) for _ in range(len(players))]  # 初始化结果表格

    def record_result(self, red_player, black_player, outcome):
        assert red_player in self.players, f"{red_player} is not a valid player."
        assert black_player in self.players, f"{black_player} is not a valid player."

        red_index = self.players.index(red_player)
        black_index = self.players.index(black_player)

        if outcome == BoardResult.RED_WIN:
            self.results[red_index][black_index] += 2
            self.results[black_index][red_index] += 0
        elif outcome == BoardResult.BLACK_WIN:
            self.results[red_index][black_index] += 0
            self.results[black_index][red_index] += 2
        elif outcome == BoardResult.NOT_END:
            self.results[red_index][black_index] += 1
            self.results[black_index][red_index] += 1

    def calculate_scores(self):
        self.scores = {player: sum(row) for player, row in zip(self.players, self.results)}

    def print_table(self):
        headers = [""] + self.players + ["Total"]
        table = [[self.players[i]] + row + [self.scores[self.players[i]]] for i, row in enumerate(self.results)]
        table = sorted(table, key=lambda x: x[-1], reverse=True)  # 按成绩从高到低排序
        print(tabulate(table, headers=headers, tablefmt="simple_grid"))


if __name__ == '__main__':
    players = ["Alice", "Bob", "Charlie", "David"]
    tournament = ChessTournament(players)

    tournament.record_result("Alice", "Bob", BoardResult.RED_WIN)
    tournament.record_result("Alice", "Charlie", BoardResult.NOT_END)
    tournament.record_result("Alice", "David", BoardResult.BLACK_WIN)
    tournament.record_result("Bob", "Charlie", BoardResult.RED_WIN)
    tournament.record_result("Bob", "David", BoardResult.RED_WIN)
    tournament.record_result("Charlie", "David", BoardResult.BLACK_WIN)

    tournament.calculate_scores()

    tournament.print_table()