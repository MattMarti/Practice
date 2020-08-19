import subprocess as sub

def main():
    p = sub.Popen(["py", "reverse.py"], stdin=sub.PIPE, stdout=sub.PIPE)
    
    while (True):
        str_in = input()
        bytes_in = bytes(str_in + '\n', 'utf-8')
        
        p.stdin.write(bytes_in)
        p.stdin.flush()
        
        bytes_out = p.stdout.readline()
        str_out = str(bytes_out, 'utf-8')
        print(str_out[0:-1])
        
        bytes_out = p.stdout.readline()
        str_out = str(bytes_out, 'utf-8')
        print(str_out[0:-1])
        
        print('')
        
        if str_in == 'quit':
            break
        #
    #
    p.stdout = None
    p.stderr = None
    p.kill()
#

if __name__ == '__main__':
    main()
#