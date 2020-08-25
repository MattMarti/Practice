from pyee import BaseEventEmitter

class MyClass(BaseEventEmitter):

    def run(self):
        for i in range(0, 20):
            if not i % 4:
                self.emit('callback', i)

mc = MyClass()

@mc.on('callback')
def my_callback(i):
    print("i =", i)
    
def main():
    mc.run()

if __name__ == '__main__':
    main()
