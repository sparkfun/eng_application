#!/usr/bin/ruby

require 'json'

file = File.read('json/calls.json')
file = file.gsub('BLOCK OF ','')

#substitute when a word boundary (\b)
#is followed by a space 
#and a slash /
#and another space
#and another word boundary (\b)
 
file = file.gsub(/\b \/ \b/, " and ")
data_hash = JSON.parse(file)

data_hash.each do |line|
    #1) Strip out I/P (in progress) from call_type.
    clean_line = line['call_type'].sub(/ I\/P/i,'') 
        
    #2) Strip out things preceding a dash.
    clean_line = clean_line.sub(/^[A-Z0-9]*\s*-\s*/,'')
        
    #3) Strip out the letter C at the end of line (Think this means cold.)
    clean_line = clean_line.sub(/ C$| Cold$/,'')
	if clean_line.eql?("Assault")  ||
	   clean_line.eql?("Burglary") ||
	   clean_line.eql?("Indecent Exposure") ||
	   clean_line.eql?("Stabbing") ||
	   clean_line.eql?("Theft") ||
	   clean_line.eql?("Vandalism")
		 puts line['loc']
	end
end
