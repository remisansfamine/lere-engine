import engine

go = engine.GameObject()
tr = engine.Transform(go)
rb = engine.Rigidbody(go)

print(tr)
print(tr.m_position)
print(tr.m_position.x)
print(tr.m_position.y)
print(tr.m_position.z)
print(tr.m_scale.x)

tr.m_position.x += 1
tr.m_position.y -= 2

print(tr.m_position.x)
print(tr.m_position.y)

print("-------------")
print(rb.mass)
rb.mass += 9
print(rb.mass)