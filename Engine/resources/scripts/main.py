import debug
import input

def update():
    debug.info("global grger")

class main:
    def __init__(self):
        print("Construction")

    def start(self, inputs):
        inputs.addButton("Jump", 32)

    def lateUpdate(self):
        debug.info("oui")

    def update(self):
        debug.info("fefe")

        if (input.getButtonDown("Jump")):
            debug.info("local test true")
        else:
            debug.info("local test false")