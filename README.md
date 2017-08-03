# laser-tag-system
Paint Ball or Laser Tag game system that receives input from sensors or that can be used to analyse statistics from the game.

Laser Tag is a real world game that simulates weapon warfare using equipment such ass special vests and laser emitting guns to detect when one player shoots towards another player and when a players passes by a zone in the arena.

The program considers a Laser Tag championship consisting of more rounds (games).

It reads from input, in this order:
1. The number of sensors and their relative localisation (the distance between any two sensors);
2. The names of the players in the two teams separated on two lines.
Then for every game(round):
1. A game beginning keyword: GAME_X where X is the number of the game that is about to begin;
-> Input from localisation sensors respecting the format:
sensor_number: detected_player_name
-> Input from laser guns + vests respecting the format:
shooter_player -> shot_player


The program outputs the following:
-> The top 5 shooters
-> The top 5 map explorers
-> The top 5 duel pairs (players that have shot eachother the most)
-> The final score of the two teams

Observations:
-> The program considers that a round ends right before a new one begins, apart from the last round which ends when the END_CHAMPIONSHIP command is given. At that point, the program calculates a winning chance and determines which team would have won taking into account the remaining alive players and their scores. (You usually play for an amount of time, not for an amount of rounds, in real life);
-> A player wins 2 points for shooting an enemy and looses 5 points for shooting a team-mate;
-> I've used a hash table for storing and easily accessing the information related to players and two graphs: one for the sensors and their relative positions and one for the number of duels between a pair of players.
