#! /usr/bin/env python

import os
import sys
import json

# Guides the user through translation of a series of ASCIILib text files.
# This script is called as follows:
#   translate.py [language] [file1] [file2] ...

if __name__ == "__main__":
    language_name = sys.argv[1]

    # Make a directory for the language
    if not os.path.exists(language_name):
        os.makedirs(language_name)

    print('''Translation: Read each paragraph carefully as it appears. Type
          the paragraph in your own language, marking any places where you're
          uncertain or have left your translation unfinished with the symbol @,
          then press enter.''')
    print('---')

    # Iterate through the JSON files
    for filename in sys.argv[2:]:
        print('Beginning to translate ' + filename + '...')

        # Load the original JSON
        original_file = open(filename, 'r')
        text_dict = json.load(original_file)
        translated_dict = {}

        # Iterate through each paragraph in original order

        # Have to reload the file to read it line by line
        original_file = open(filename, 'r')

        # Also check if there's already a partially- or fully-translated file
        # for this one already
        stripped_filename = filename[filename.rfind('/'):]
        translated_filename = language_name + '/' + stripped_filename

        if (os.path.exists(translated_filename)):
            translated_dict = json.load(open(translated_filename, 'r'))

        for line in original_file.readlines():
            # Check that the line contains a JSON key/value pair
            if line.count('"'):
                first_quote_index = line.find('"')
                second_quote_index = line.find('"', first_quote_index+1)

                key = line[first_quote_index+1:second_quote_index]

                # Don't ask the user to re-translate what they already have
                if key in translated_dict:
                    continue

                paragraph = text_dict[key]

                print(paragraph)

                translated_paragraph = raw_input('Your translation: ')
                translated_dict[key] = translated_paragraph

                # Save the translated file at every step, to avoid losing work!
                json.dump(translated_dict, open(translated_filename, 'w'))

        print('Done translating ' + filename + '!')

    print('Done translating all of the files!')
