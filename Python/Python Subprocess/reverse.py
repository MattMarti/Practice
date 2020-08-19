import sys

def main():
    while True:
        str = input()
        sys.stdin.flush()
        
        print(str[::-1])
        sys.stdout.flush()
        
        print(str[::-1] + str)
        sys.stdout.flush()
    #
#

if __name__ == '__main__':
    main()
#