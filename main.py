import debug
import input

def update():
    debug.info("global a1")
    debug.info("global a2")
    debug.info("global a3")
    debug.info("global a4")
    debug.info("global a5")
    debug.info("global a6")

class main:
    def start(self, inputs):
        inputs.addButton("Jump", 32)

    def lateUpdate(self):
        debug.info("oui")

    count = 0
    def update(self):
        print(self.count)
        self.count += 1
