
import datetime;

'''
Class Features:
- Methods
- Initialization
- Help text
'''
class User:
    """A member of FriendFace. For now we are
    only storing their name and birthday.
    But soon we will store an uncomfortable
    amount of user information."""
    
    def __init__(self, full_name, birthday): # Called every an object is created of this class
        """Doc comment for function __init__()"""
        self.name = full_name;
        self.birthday = birthday; # yyyymmdd
    
    
        # Extract first and last names
        name_pieces = full_name.split(" ")
        self.first_name = name_pieces[0]
        self.last_name = name_pieces[-1]

    #pass # A way to type a line that does nothing
    
    def age(self):
        """Return the age of the user in years"""
        today = datetime.date(2001, 5, 12);
        yyyy = int(self.birthday[0:4])
        mm = int(self.birthday[4:6])
        dd = int(self.birthday[6:8])
        dob = datetime.date(yyyy, mm, dd) #Date of birth
        age_in_days = (today - dob).days;
        age_in_years = age_in_days / 365
        return int(age_in_years)
#

if __name__ == "__main__":
    user = User("Dave Bowman", "19710315")
    print(user.name)
    print(user.birthday)
    print(user.first_name)
    print(user.last_name)
    print(user.age())
#