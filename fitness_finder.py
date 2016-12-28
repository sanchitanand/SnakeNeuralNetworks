

import re

def min_and_max(iterable):
    iterator = iter(iterable)

    # Assumes at least two items in iterator
    minim, maxim = sorted((next(iterator), next(iterator)))

    for item in iterator:
        if item < minim:
            minim = item
        elif item > maxim:
            maxim = item

    return (minim, maxim)


with open('gen_20', 'r') as f:
    g = (re.search(r'Fitness: [-+]?[0-9]*\.?[0-9]*',line,re.I) for line in f) 
    values = (float(v.group(1)) for v in g)
    minim, maxim = min_and_max(values)

print(maxim)
print(minim)