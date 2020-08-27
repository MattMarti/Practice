import av
import av.datasets

content = 'The-Bee-Movie-but-gets-faster.mp4'

with av.open(content) as container:
    
    # Signal that we only want to look at keyframes
    stream = container.streams.video[0]
    stream.codec_context.skip_frame = 'NONKEY'
    
    for frame in container.decode(stream):
        print(frame)
        
        frame.to_image().save(
            'bee.{:04d}.jpg'.format(frame.pts),
            quality = 80,
        )