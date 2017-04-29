import tensorflow as tf

stringNode = tf.constant("HelloWorld")
floatNode1 = tf.constant(3.0)
intNode2 = tf.constant( 2, tf.uint8 )
sess = tf.Session()
print(sess.run([stringNode, floatNode1]))

node3 = tf.add(floatNode1, tf.to_float(intNode2, name="ToFloat"))
print(sess.run(node3))

a = tf.placeholder(tf.float32)
b = tf.placeholder(tf.float32)
adder_node = a + b
print(sess.run(adder_node, {a: 2, b: 3}))

adder_and_node = adder_node * 3
print(sess.run(adder_and_node, {a:3, b: 4.5}))

W = tf.Variable([.3, 0.3], tf.float32)
b = tf.Variable([-0.3], tf.float32)
x = tf.placeholder(tf.float32)
linear_model = W*x + b;

# Call initializer to initialize all variable 
init = tf.global_variables_initializer()
sess.run(init)

# Run linear model
print(sess.run(linear_model, {x:[[1, 2], [3, 4]]}))

#SSE
y = tf.placeholder(tf.float32)
squared_deltas = tf.square(linear_model - y)
loss = tf.reduce_sum(squared_deltas)
print(sess.run(loss, {x:[[1, 2], [3, 4]], y:[[0, 0.25], [0.6, 1]]}))

# Changing W, b using tf.assign
fixW = tf.assign(W, [-1, -1])
fixB = tf.assign(b, [1])
sess.run([fixW, fixB])
print(sess.run(loss, {x:[[1, 2], [3, 4]], y:[[0, 0.25], [0.6, 1]]}))


# Train a gradient descent optimizer
optimizer = tf.train.GradientDescentOptimizer(0.01) # eta
train = optimizer.minimize(loss)

for i in range(100):
	print(sess.run(train, {x:[[1, 2], [3, 4]], y:[[0, 0.25], [0.6, 1]]}))
print(sess.run([W, b]))
