import asyncio
import time

async def say_after(delay, str):
    await asyncio.sleep(delay)
    print(str)

async def printfunc():
    print(f"Start: {time.strftime('%X')}")
    
    task_world = asyncio.create_task(say_after(2, "world"))
    task_hello = asyncio.create_task(say_after(1, "hello"))
    
    await task_world # Call print "world" first
    
    await task_hello # Call print "Hello" second
    
    print(f"End: {time.strftime('%X')}")
    
asyncio.run(printfunc())
