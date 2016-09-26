import re

text = "The sky may be sunny or cloudy, but still you should try to do your best to achieve your tiny joy."

x = re.findall(r"[\w]*y\b",text)
print(x)


