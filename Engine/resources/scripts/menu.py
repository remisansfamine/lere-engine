import debug

## Global functrion example
def update():
    for i in range(7):
        debug.info("global " + str(i))

## Script instance example (does not work completly)
class main:
    def start(self, inputs):
        debug.info("start")

    def lateUpdate(self):
        debug.info("lateUpdate")

    def update(self):
        debug.info("update")
