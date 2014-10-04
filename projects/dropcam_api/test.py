#!/usr/bin/python -u

#NOTE: using unbuffered IO with -u arg above


from dropcam import Dropcam
d = Dropcam("patchcam", "c1himera")
c = d.cameras()[0]
c.save_image("image.jpg")
