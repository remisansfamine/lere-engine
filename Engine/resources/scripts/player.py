import engine

class player(engine.Component) :
    def start(self) :
        print("starting")
        self.gameobject = engine.getHost()

    def update(self) :
        print("updating")
        print(self.gameobject.getComponent<engine.Transform>().m_position.x)