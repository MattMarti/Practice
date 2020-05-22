import json;

def main():
    '''
    json_example.py
    
    Socratica tutorial video on how to use json strings and files to push
    data from one place to another.
    
    @author: Matt Marti
    @date: 2019-06-15
    '''
    
    # First, we can load data from the example file
    json_file = open("json.json", "r");
    movie = json.load(json_file);
    
    # Show user what was loaded from file
    print("Displaying data from ""json.json""");
    print(movie)
    print(" ");
    print("Displaying the 'title' field");
    print(movie["title"]);
    
    # Now we practice with a string
    json_str = """ {"title" : "Tron: Legacy",
            "composer" : "Daft Punk",
            "release_year" : 2010,
            "budget" : 170000000,
            "actors" : null,
            "won_oscar" : false} """
    tron = json.loads(json_str);
    
    # Display the string
    print("Displaying data from string");
    print(tron)
    print(" ");
    print("Displaying the 'title' field");
    print(tron["title"]);
    print(" ")
    
    # Put the movie data, which is a python object, into a json sytle format
    jsondata = json.dumps(movie);
    print("json formatted data:")
    print(jsondata);
    print(" ")
    
    # Create our own json object and write it to a file
    movie2 = {}; # Create dictionary
    movie2["title"] = "Minority Report";
    movie2["director"] = "Steven Spielberg";
    movie2["composer"] = "John Williams";
    movie2["actors"] = ["Tom Cruise", "colin Farrell", 
        "Samantha Morton", "Max von Sydow"]
    movie2["is_awesome"] = True;
    movie2["budget"] = 102000000;
    movie2["cinematographer"] = "Janusz Kami\u0144ski" # Latin small 'n' with acute
    # This uses a unicode character, to complicate things a little
    
    # Open a file and write to it
    file2 = open("json_out.json", "w", encoding="utf-8");
    json.dump(movie2, file2, ensure_ascii=False);
    file2.close();
    print("Writing following json to file:")
    print(json.dumps(movie2, ensure_ascii=False))
#

if __name__ == "__main__":
    main();
#