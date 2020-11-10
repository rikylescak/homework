#!/usr/bin/env python3
import sys, getopt, random

global shuffle_lines
shuffle_lines = False

def get_lyrics_list(filename):						#strips lines in file ad returns them
	with open(filename, "r") as file:
		return [line.strip('\n') for line in file]

def print_lyrics(lyrics_list, shuffle_func):		#prints lyrics by every line in every paragraph
	shuffle_func(lyrics_list, shuffle_lines)
	for paragraph in lyrics_list:
		for line in paragraph:
			print(line)
		print('')

def parse_lyrics(lyrics_list):						#function parses lyrics and return them parsed
	parsed_lyrics = []
	paragraph = []
	
	for line in lyrics_list:
		if (line == ""):
			parsed_lyrics.append(paragraph)
			paragraph = []
		else:
			paragraph.append(line)
			
	parsed_lyrics.append(paragraph)	
	return parsed_lyrics
			
def shuffle_lyrics(lyrics_list, shuffle_lines):		#shuffles paragraphs in lyrics, if shuffle_lines is set to True,it will shuffle lines too
	random.shuffle(lyrics_list)
	if shuffle_lines == True:
		[random.shuffle(paragraph) for paragraph in lyrics_list]

def print_help():									#prints help text
	print(	'''\nProgram will reshuffle lyrics in a file and print them on STDOUT
			\t./shuffle_lyrics.py [OPTIONS]
			-f, --file=[PATH]
			\t Path to file with lyrics.
			-l, --lines
			\t Will shuffle also lines in paragraphs. Optional\n''')
			
filename = ""
argv = sys.argv[1:]
try:												#process imput arguments
	opts, args = getopt.getopt(argv,"lf:h:",["lines", "file=", "help"])
except getopt.GetoptError:
	print_help()
	sys.exit()
	
for opt, arg in opts:
	if opt in ("-l", "--lines"):
		shuffle_lines = True
	elif opt in ("-f", "--file"):
		filename = arg;
	elif opt in ("-h", "--help"):
		print_help()
		sys.exit()

if filename == "":
	print("You need to specify a file.")
	sys.exit()
		
print_lyrics(parse_lyrics(get_lyrics_list(filename)), shuffle_lyrics)
	
