class Pizza(object):
    def __init__(self):
        self.toppings = []
    def __call__(self, topping):
        # when using '@instance_of_pizza' before a function def
        # the function gets passed onto 'topping'
        self.toppings.append(topping())
    def __repr__(self):
        return str(self.toppings)



@Pizza
def cheese():
    pass


print pizza