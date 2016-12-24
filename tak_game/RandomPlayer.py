import sys
import pdb
import time
import random
import copy
import time

COUNT=0
DEPTH=3
BRANCHING_FACTOR=0
VISITED=[]
VERTICES=[]
one_comp=[]
class Game:

	class Player:

		def __init__(self, flats, capstones):
			self.flats = flats
			self.capstones = capstones
			self.pieces = 0
			self.flatpieces = 0
			self.centerpieces = 0
			self.connected_pieces = 0
		
	def __init__(self, n):
		self.n = n
		self.total_squares = n * n
		self.board = [[] for i in xrange(self.total_squares)]
		self.turn = 0
		if n == 5:
			self.max_flats = 21
			self.max_capstones = 1
		elif n == 6:
			self.max_flats = 30
			self.max_capstones = 1
		elif n == 7:
			self.max_flats = 40
			self.max_capstones = 1
		self.max_movable = n
		self.max_down = 1
		self.max_up = n
		self.max_left = 'a'
		self.max_right = chr(ord('a') + n - 1)
		self.moves = 0
		self.players = []
		self.players.append(Game.Player(self.max_flats, self.max_capstones))
		self.players.append(Game.Player(self.max_flats, self.max_capstones))
		self.all_squares = [self.square_to_string(i) for i in xrange(self.total_squares)]

	def square_to_num(self, square_string):
		''' Return -1 if square_string is invalid
		'''
		
		if len(square_string) != 2:
			return -1
		if not square_string[0].isalpha() or not square_string[0].islower() or not square_string[1].isdigit():
			return -1
		row = ord(square_string[0]) - 96
		col = int(square_string[1])
		if row < 1 or row > self.n or col < 1 or col > self.n:
			return -1
		return self.n * (col - 1) + (row - 1)

	def square_to_string(self, square):
		'''Convert square number to string
		'''
		if square < 0 or square >= self.total_squares:
			return ''
		row = square % self.n
		col = square / self.n
		return chr(row + 97) + str(col + 1)

	def execute_move(self, move_string):
		'''Execute move
		'''
		n = self.n
		if self.turn == 0:
			self.moves += 1
		if self.moves != 1:
			current_piece = self.turn
		else:
			current_piece = 1 - self.turn
		if move_string[0].isalpha():
			square = self.square_to_num(move_string[1:])
			if move_string[0] == 'F':
				self.board[square].append((current_piece, move_string[0]))
				self.players[current_piece].flats -= 1
				self.players[current_piece].pieces += 1
				self.players[current_piece].flatpieces += 1
			elif move_string[0] == 'S':
				self.board[square].append((current_piece, move_string[0]))
				self.players[current_piece].flats -= 1
				self.players[current_piece].pieces += 1
			elif move_string[0] == 'C':
				self.board[square].append((current_piece, move_string[0]))
				self.players[current_piece].capstones -= 1
				self.players[current_piece].pieces += 1
				self.players[current_piece].flatpieces += 1
			if( square >= n and square % n != 0 and square % n != n-1 and n <= n*(n-1)) :
				self.players[current_piece].centerpieces += 1
			if( move_string[0] == 'F' or move_string[0] == 'C'):
				arr = [square-1,square+1,square-n,square+n]
				for j in xrange(4):
					new_pos = arr[j]
					if (new_pos >=0 and new_pos<self.n * self.n):
						if (len(self.board[new_pos])!=0):
							if (self.board[new_pos][-1][0]==current_piece and self.board[new_pos][-1][1] != 'S'):
								self.players[current_piece].connected_pieces += 1
		elif move_string[0].isdigit():
			count = int(move_string[0])
			square = self.square_to_num(move_string[1:3])
			direction = move_string[3]
			if direction == '+':
				change = self.n
			elif direction == '-':
				change = -self.n
			elif direction == '>':
				change = 1
			elif direction == '<':
				change = -1
			prev_square = square
			iden = self.board[square][-1]
			self.players[iden[0]].pieces -= 1
			if iden[1] != 'S':
				self.players[iden[0]].flatpieces -= 1
			if( square >= n and square % n != 0 and square % n != n-1 and square <= n*(n-1)) :
				self.players[iden[0]].centerpieces -= 1
			for i in xrange(4, len(move_string)):
				next_count = int(move_string[i])
				next_square = prev_square + change	
				if (len(self.board[next_square]) > 0):
					iden = self.board[next_square][-1]
				if (len(self.board[next_square]) > 0) and (self.board[next_square][-1][1] == 'S'):
					self.board[next_square][-1] = (self.board[next_square][-1][0], 'F')
					self.players[iden[0]].flatpieces += 1

				if (len(self.board[next_square]) > 0):
					self.players[iden[0]].pieces -= 1
					if iden[1] != 'S':
						self.players[iden[0]].flatpieces -= 1
					if( next_square >= n and next_square % n != 0 and next_square % n != n-1 and next_square <= n*(n-1)) :
						self.players[iden[0]].centerpieces -= 1
				
				if next_count - count == 0:
					self.board[next_square] += self.board[square][-count:]
				else:
					self.board[next_square] += self.board[square][-count:-count+next_count]

				iden = self.board[next_square][-1]
				self.players[iden[0]].pieces += 1
				if iden[1] != 'S':
					self.players[iden[0]].flatpieces += 1
				if( next_square >= n and next_square % n != 0 and next_square % n != n-1 and next_square <= n*(n-1)) :
					self.players[iden[0]].centerpieces += 1
				
				prev_square = next_square
				count -= next_count
			count = int(move_string[0])
			self.board[square] = self.board[square][:-count]
			if (len(self.board[square]) > 0):
				iden = self.board[square][-1]
				self.players[iden[0]].pieces += 1
				if iden[1] != 'S':
					self.players[iden[0]].flatpieces += 1
				if( square >= n and square % n != 0 and square % n != n-1 and square <= n*(n-1)) :
					self.players[iden[0]].centerpieces += 1
		self.turn = 1 - self.turn

	def partition(self, n):
		'''Generates all permutations of all partitions
		of n
		'''

		part_list = []
		part_list.append([n])
		for x in xrange(1, n):
			for y in self.partition(n - x):
				part_list.append([x] + y)
		return part_list

	def check_valid(self, square, direction, partition):
		'''For given movement (partition), check if stack on
		square can be moved in direction. Assumes active player
		is topmost color
		'''
		if direction == '+':
			change = self.n
		elif direction == '-':
			change = -self.n
		elif direction == '>':
			change = 1
		elif direction == '<':
			change = -1
		for i in xrange(len(partition)):
			next_square = square + change * (i + 1)
			if len(self.board[next_square]) > 0 and self.board[next_square][-1][1] == 'C':
				return False
			if len(self.board[next_square]) > 0 and self.board[next_square][-1][1] == 'S' and i != len(partition) - 1:
				return False
			if i == len(partition) - 1 and len(self.board[next_square]) > 0 and self.board[next_square][-1][1] == 'S' and partition[i] > 1:
				return False
			if i == len(partition) - 1 and len(self.board[next_square]) > 0 and self.board[next_square][-1][1] == 'S' and self.board[square][-1][1] != 'C':
				return False
		return True

	def generate_stack_moves(self, square):
		'''Generate stack moves from square
		Assumes active player is topmost color
		'''

		all_moves = []
		r = square % self.n
		c = square / self.n
		size = len(self.board[square])
		dirs = ['+', '-', '<', '>']
		up = self.n - 1 - c
		down = c
		right = self.n - 1 - r
		left = r
		rem_squares = [up, down, left, right]
		for num in xrange(min(size, self.n)):
			part_list = self.partition(num + 1)
			for di in range(4):
				part_dir = [part for part in part_list if len(part) <= rem_squares[di]]
				# #sys.stderr.write(self.all_squares[square] + ' ' + dirs[di] + ' ' + str(part_dir) + '\n')
				for part in part_dir:
					if self.check_valid(square, dirs[di], part):
						part_string = ''.join([str(i) for i in part])
						all_moves.append(str(sum(part)) + self.all_squares[square] + dirs[di] + part_string)
		return all_moves

	def generate_all_moves(self, player):
		'''Generate all possible moves for player
		Returns a list of move strings
		'''
		all_moves = []
		for i in xrange(len(self.board)):
			if len(self.board[i]) == 0:
				if self.players[player].flats > 0:
					all_moves.append('F' + self.all_squares[i])
				if self.moves != player and self.players[player].flats > 0:
					all_moves.append('S' + self.all_squares[i])
				if self.moves != player and self.players[player].capstones > 0:
					all_moves.append('C' + self.all_squares[i])
		for i in xrange(len(self.board)):
			if len(self.board[i]) > 0 and self.board[i][-1][0] == player and self.moves != player:
				all_moves += self.generate_stack_moves(i)
		return all_moves
		
class RandomPlayer:
	def pieces_on_top(self, curgame):
		if self.player==0:
			return curgame.players[0].pieces - curgame.players[1].pieces, curgame.players[0].flatpieces - curgame.players[1].flatpieces, curgame.players[0].centerpieces - curgame.players[1].centerpieces
		elif self.player==1:
			return curgame.players[1].pieces - curgame.players[0].pieces, curgame.players[1].flatpieces - curgame.players[0].flatpieces, curgame.players[1].centerpieces - curgame.players[0].centerpieces

	def connected_pieces(self, curgame):
		if self.player==0:
			return curgame.players[0].connected_pieces - curgame.players[1].connected_pieces
		elif self.player==1:
			return curgame.players[1].connected_pieces - curgame.players[0].connected_pieces

	def num_in_stack(self, curgame):
		mypieces=0
		oppieces=0
		for i in xrange(len(curgame.board)):
			if (len(curgame.board[i])>1):
				if (curgame.board[i][-1][0]==self.player):
					for j in xrange(len(curgame.board[i])-1):
						if curgame.board[i][j][0]==self.player:
							mypieces += 1

				else:
					for j in xrange(len(curgame.board[i])-1):
						if curgame.board[i][j][0]!=self.player:
							oppieces += 1
		return mypieces - oppieces

	def DFS (self,v,i):
		global one_comp
		global VISITED
		global VERTICES
		VISITED[i]=1
		one_comp.append(v)
		n = self.n
		if(v%n==0):
			nbrs = [v+1,v-self.n,v+self.n]
		elif(v%n==n-1):
			nbrs = [v-1,v-self.n,v+self.n]
		elif(v/n==0):
			nbrs = [v-1,v+1,v+self.n]
		elif(v/n==n-1):
			nbrs = [v-1,v+1,v-self.n]
		else:
			nbrs = [v+1,v-1,v+self.n,v-self.n]
		for nbr in nbrs:
			if nbr in VERTICES:
				j = VERTICES.index(nbr)
				if VISITED[j]==0:
					self.DFS(nbr,j)

	def max_road_length(self,curgame):
		global VISITED
		global VERTICES
		global one_comp
		global COUNT
		VERTICES = []
		for i in xrange(len(curgame.board)):
			if (len(curgame.board[i])>0):
				if (curgame.board[i][-1][0]==self.player and curgame.board[i][-1][1]!='S'):
					VERTICES.append(i)
		# if COUNT==10:
					#sys.stderr.write('Vertices '+str(VERTICES)+'\n')
		VISITED = []
		for i in xrange(len(VERTICES)):
			VISITED.append(0)

		one_comp=[]
		max_road_length1 = 0
		maxi = 0
		mini = self.n * self.n
		for i in xrange(len(VERTICES)):
			if VISITED[i]==0:
				self.DFS(VERTICES[i],i)
				# if COUNT==10:
					#sys.stderr.write('One component '+str(one_comp)+'\n')
				arr = [mini,maxi,mini,maxi]
				for some in one_comp:
					r = some%self.n
					c = some/self.n
					if r < arr[0]:
						arr[0] = r
					if r > arr[1]:
						arr[1] = r
					if c < arr[2]:
						arr[2] = c
					if c > arr[3]:
						arr[3] = c
					if arr[1]-arr[0] > max_road_length1:
						max_road_length1 = arr[1] - arr[0]
					if arr[3]-arr[2] > max_road_length1:
						max_road_length1 = arr[3] - arr[2]
				one_comp=[]


		# Opponent road size
		VERTICES = []
		for i in xrange(len(curgame.board)):
			if (len(curgame.board[i])>0):
				if (curgame.board[i][-1][0]==1-self.player and curgame.board[i][-1][1]!='S'):
					VERTICES.append(i)
		VISITED = []
		for i in xrange(len(VERTICES)):
			VISITED.append(0)
		one_comp=[]
		max_road_length2 = 0
		maxi = 0
		mini = self.n * self.n
		for i in xrange(len(VERTICES)):
			if VISITED[i]==0:
				self.DFS(VERTICES[i],i)
				# if COUNT==10:
					#sys.stderr.write('One component '+str(one_comp)+'\n')
				arr = [mini,maxi,mini,maxi]
				for some in one_comp:
					r = some%self.n
					c = some/self.n
					if r < arr[0]:
						arr[0] = r
					if r > arr[1]:
						arr[1] = r
					if c < arr[2]:
						arr[2] = c
					if c > arr[3]:
						arr[3] = c
					if arr[1]-arr[0] > max_road_length2:
						max_road_length2 = arr[1] - arr[0]
					if arr[3]-arr[2] > max_road_length2:
						max_road_length2 = arr[3] - arr[2]
				one_comp=[]
		return max_road_length1, max_road_length2
		

	def unplayed(self,curgame):
		myunflats = curgame.players[self.player].flats
		opunflats = curgame.players[1-self.player].flats
		return myunflats - opunflats

	def evaluate(self, curgame):
		global COUNT
		COUNT+=1
		# #sys.stderr.write(str(COUNT)+'\n')
		f1,f2,f3 = self.pieces_on_top(curgame)
		f4 = self.connected_pieces(curgame)
	#	f4 = self.num_in_stack(curgame)		#Number of my stones in my stack
		f5, f6 = self.max_road_length(curgame)
		f7 = self.unplayed(curgame)
		#if COUNT==10:	
			#sys.stderr.write(str(COUNT) + ' ' + str(curgame.board) + '\n')
			#sys.stderr.write(str(f5)+'\n')
		return 0.2*f1 + f2 + 2*f3 + 80*f4 + 100*f5 + (-1)*100*f6 + f7
		
	def minimax(self,curGame, depth, minValue, maxValue,GameMove):
		global DEPTH
		global COUNT
		global BRANCHING_FACTOR
		copyGame = copy.deepcopy(curGame)		#Seperately instantiate??
		if (depth==0):
			return self.evaluate(curGame),GameMove
		if depth % 2 == DEPTH%2:
			bestValue = minValue
			bestMove = ""
			allmove_utility = []
			all_moves = copyGame.generate_all_moves(self.player)
			BRANCHING_FACTOR = len(all_moves)
			for i in xrange(len(all_moves)):
				copyGame.execute_move(all_moves[i])
				allmove_utility.append(self.evaluate(copyGame))
				copyGame = copy.deepcopy(curGame)
			moveMap = {}
			for i in xrange(len(all_moves)):
				moveMap[allmove_utility[i]] = all_moves[i]

			for key in reversed(sorted(moveMap)):
				copyGame.execute_move(moveMap[key])
				curValue, curMove = self.minimax(copyGame,depth-1,bestValue,maxValue,moveMap[key])
				copyGame = copy.deepcopy(curGame)
				# if depth==3:
					# #sys.stderr.write('Possible moves: ' + str(moveMap[key]) + ' value : '+str(curValue) + '\n')
				if curValue > bestValue:
					bestValue = curValue
					if depth==DEPTH:
						bestMove = moveMap[key]
					#~ #sys.stderr.write('Move is '+str(GameMove) + ' depth is ' + str(depth))
				if bestValue > maxValue:
					#sys.stderr.write(str(depth))
					return maxValue,curMove			#Make maxmove??
			return bestValue, bestMove
			
		else:
			bestValue = maxValue
			bestMove = ""
			allmove_utility = []
			all_moves = copyGame.generate_all_moves(1-self.player)
			for i in xrange(len(all_moves)):
				copyGame.execute_move(all_moves[i])
				allmove_utility.append(self.evaluate(copyGame))
				copyGame = copy.deepcopy(curGame)
			moveMap = {}
			for i in xrange(len(all_moves)):
				moveMap[allmove_utility[i]] = all_moves[i]
			for key in reversed(sorted(moveMap)):
				copyGame.execute_move(moveMap[key])
				curValue, curMove = self.minimax(copyGame,depth-1,minValue,bestValue,moveMap[key])
				copyGame = copy.deepcopy(curGame)
				if curValue < bestValue:
					bestValue = curValue
					bestMove = curMove
				if bestValue < minValue:
					#sys.stderr.write(str(depth))
					return minValue,curMove
			return bestValue, bestMove

			
	def __init__(self):
		data = sys.stdin.readline().strip().split()
		self.player = int(data[0]) - 1
		self.n = int(data[1])
		self.time_left = int(data[2])
		self.game = Game(self.n)
		self.play()

	def play(self):
		global COUNT
		global DEPTH
		global BRANCHING_FACTOR	
		start_time = time.time()
		if self.player == 1:
			move = sys.stdin.readline().strip()
			self.game.execute_move(move)
			#sys.stderr.write("banana")
			#sys.stderr.write('Chosen move: ' + move)
		while True:
			curTime = time.time()
			time_elapsed = start_time - curTime
			time_left = self.time_left - time_elapsed
			#Count number of empty squares on board
			empty_squares = 0
			for i in xrange(len(self.game.board)):
				if len(self.game.board[i]) == 0:
					empty_squares+=1
			expected_time_for_move = time_left/(10 + empty_squares)
			elapsed_time_for_move = 0
			# #sys.stderr.write('Time Left '+ str(self.time_left))
			DEPTH=2
			BRANCHING_FACTOR=100
			COUNT=0
			#sys.stderr.write('Expected time '+str(2*expected_time_for_move/(1+BRANCHING_FACTOR)))
			move=""
			while (elapsed_time_for_move*(1+BRANCHING_FACTOR) < 3*expected_time_for_move):
				COUNT=0
				#sys.stderr.write('Elapsed time' + str(elapsed_time_for_move)+'\n')
				#sys.stderr.write('BRANCHING_FACTOR ' + str(BRANCHING_FACTOR)+'\n')
				value1,move = self.minimax(self.game, DEPTH, -1000, 1000, None)
				DEPTH+=1
				#sys.stderr.write('Depth '+str(DEPTH)+'\n')
				elapsed_time_for_move = time.time() - curTime
				#sys.stderr.write('Elapsed time '+str(elapsed_time_for_move)+'\n')
			#sys.stderr.write('Move is '+str(move)+'\n')
			self.game.execute_move(move)
			move = move + '\n'
			sys.stdout.write(move)
			sys.stdout.flush()
			move = sys.stdin.readline().strip()
			self.game.execute_move(move)

random_player = RandomPlayer()
