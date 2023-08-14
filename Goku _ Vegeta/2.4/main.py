import sensor, image, time
from pyb import UART
import math

uart = UART(3, 19200)

orange_threshold = (40, 69, 31, 77, 28, 61)  # Orange ball color threshold
size_threshold = 2000  # Area of blob

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()

def find_max(blobs):
    max_blob = None
    max_blob_size = 0
    for blob in blobs:
        if blob.area() > max_blob_size:
            max_blob = blob
            max_blob_size = blob.area()
    return max_blob

while True:
    clock.tick()
    img = sensor.snapshot()

    # Find orange blobs
    orange_blobs = img.find_blobs([orange_threshold])
    if orange_blobs:
        max_orange_blob = find_max(orange_blobs)
        orange_x = max_orange_blob.cx() - 149  # Calculate relative X coordinate
        orange_y = 93 - max_orange_blob.cy()  # Calculate relative Y coordinate with inverted sign

        # Calculate angle in radians
        angle = math.atan2(orange_y, orange_x)
        angle_degrees = math.degrees(angle)

        # Convert angle to the range of 0 to 360 degrees
        if angle_degrees < 0:
            angle_degrees += 360

        print("Orange ball coordinates: ({}, {})".format(orange_x, orange_y))
        print("Angle: {:.2f} degrees".format(angle_degrees))

        uart.write("{:.2f}\n".format(angle_degrees))

        img.draw_rectangle(max_orange_blob.rect())
        img.draw_cross(max_orange_blob.cx(), max_orange_blob.cy())
    else:
        # No orange blob detected, set angle to 0
        angle_degrees = 0
        uart.write("0.00\n")
