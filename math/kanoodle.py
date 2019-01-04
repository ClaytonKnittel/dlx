
inpu = """ A
 A
AA

 B
BB
BB

 C
 C
 C
CC

 D
 D
DD
 D

 E
 E
EE
E

F
FF

  G
  G
GGG

  H
 HH
HH

I I
III

J
J
J
J

KK
KK

 L
LLL
 L"""


class vector:

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __add__(self, v):
        return vector(self.x + v.x, self.y + v.y, self.z + v.z)

    def __sub__(self, v):
        return vector(self.x - v.x, self.y - v.y, self.z - v.z)

    def __repr__(self):
        return '<' + str(self.x) + ', ' + str(self.y) + '>'

    def quickstr3(self):
        return str(self.x) + ',' + str(self.y) + ',' + str(self.z)

    def quickstr(self):
        return str(self.x) + ',' + str(self.y)

    def num(self):
        return self.x + 5 * self.y + 25 * self.z

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z

    def __le__(self, other):
        if self.x < other.x:
            return True
        if self.x > other.x:
            return False
        if self.y < other.y:
            return True
        if self.y > other.y:
            return False
        if self.z < other.z:
            return True
        else:
            return False

    def __hash__(self):
        r = 31
        r = 17 * r + self.x
        r = 17 * r + self.y
        return r


pfill = .8

class setwrap:

    def __init__(self, se):
        self.set = se

    def __eq__(self, other):
        return self.set == other.set

    def __hash__(self):
        h = 31
        for el in self.set:
            h = 17 * h + hash(el)
        return h


class piece:

    # symmetry levels:
    #     0: no symmetries
    #     1: axial symmetry (only do first 2 orientations)
    #     2: biaxial symmetry (only flip)
    #     3: mirror image of itself (don't flip)
    #     4: axial and mirror symmetry (only do first 2 orientations and don't flip)
    #     5: entirely symmetric (don't do anything)
    def __init__(self, spots, x=0, y=0, orientation=0, flipped=False, symmetry_level=0):
        self.pos = vector(x, y, 0)
        try:
            self.spots = [vector(s[0], s[1], 0) for s in spots]
        except TypeError:
            self.spots = spots
        xx = min(v.x for v in self.spots)
        yy = min(v.y for v in self.spots)
        zz = min(v.z for v in self.spots)
        for i in range(len(self.spots)):
            self.spots[i] = self.spots[i] - vector(xx, yy, zz)

        self.orientation = orientation
        self.flipped = flipped
        self.sym = symmetry_level

    def get_spots(self):
        l = []
        for spot in self.spots:
            dx = spot.x
            dy = spot.y
            if self.flipped:
                dy *= -1
            if self.orientation == 0:
                v = vector(dx, dy, 0)
            elif self.orientation == 1:
                v = vector(-dy, dx, 0)
            elif self.orientation == 2:
                v = vector(-dx, -dy, 0)
            else:
                v = vector(dy, -dx, 0)
            l.append(self.pos + v)
        return l

    def get_all_pyramid_spots(self, w):
        al = set()
        for k in range(w):
            for i in range(w - k):
                for j in range(w - k):
                    for o in range(4):
                        for flip in [True, False]:
                            p = piece(self.spots, orientation=o, flipped=flip)
                            spots = list(p.get_spots())
                            q = vector(i, j, k)

                            for s1 in [
                                [v + q for v in spots],
                                [vector(-v.y, -v.y, v.x + v.y) + q for v in spots],
                                [vector(-v.y, -v.x, v.x + v.y) + q for v in spots],
                                [vector(-v.x, -v.y, v.x + v.y) + q for v in spots],
                                [vector(-v.x, -v.x, v.x + v.y) + q for v in spots]
                            ]:
                                if min(min(s.x, s.y, s.z) for s in s1) >= 0 and max(max(s.x + s.z, s.y + s.z, s.z) for s in s1) < w:
                                    al.add(setwrap(set(s1)))
        return sorted([sorted(list(a.set), key=lambda a: a.num()) for a in al],
                      key=lambda a: sum(125 ** (len(a) - i - 1) * v.num() for i, v in enumerate(a)))

    def gen_all_spots(self, w, h):
        if self.sym % 3 == 2:
            ors = range(1)
        elif self.sym % 3 == 1:
            ors = range(2)
        else:
            ors = range(4)
        for orientation in ors:
            if self.sym >= 3:
                flips = [False]
            else:
                flips = [False, True]

            for flipped in flips:
                p = piece(self.spots, orientation=orientation, flipped=flipped)
                spots = list(p.get_spots())
                sx = -min(v.x for v in spots)
                sy = -min(v.y for v in spots)
                wid = w - max(v.x for v in spots)
                hei = h - max(v.y for v in spots)
                for x in range(sx, wid):
                    for y in range(sy, hei):
                        yield piece(p.spots, x, y, orientation=orientation, flipped=flipped).get_spots()

    def setpos(self, newpos):
        self.pos = vector(newpos[0], newpos[1], 0)

    def insersects(self, other):
        for spot in self.get_spots():
            for spot2 in other.get_spots():
                if spot == spot2:
                    return True
        return False

    def touches(self, spot):
        v = vector(spot[0], spot[1], 0)
        for s in self.get_spots():
            if s == v:
                return True
        return False


class board:
    def __init__(self, posi, len, lon, w, h):
        self.w = w
        self.h = h
        self.dim = [len, lon]
        self.bools = [False] * (w * h)
        self.holes = []  # all the holes (just for visualization)
        self.pieces = []

    def get(self, i, j):
        if i < 0 or i >= self.w:
            return False
        if j < 0 or j >= self.h:
            return False
        return self.bools[i + j * self.w]

    def add_piece(self, p, pose=None):
        if pose is not None:
            p.setpos(pose)
        spots = p.get_spots()
        for spot in spots:
            if self.get(spot.x, spot.y):
                return False
        for spot in spots:
            self.bools[spot.x + spot.y * self.w] = True
        self.pieces.append(p)
        p.set_visible()

    def remove_last(self):
        for spot in self.pieces[-1].get_spots():
            self.bools[spot.x + spot.y * self.w] = False
        self.pieces[-1].set_invisible()
        self.pieces = self.pieces[:-1]

    def write_permutations(self, pieces, file_loc):
        all_pieces_and_orientations = []
        count = 0
        attrs = [chr(ord('A') + i) for i in range(len(pieces))]
        for x in range(self.w):
            for y in range(self.h):
                attrs += [vector(x, y, 0)]

        for piece in pieces:
            arad = [count]
            for spots in piece.gen_all_spots(self.w, self.h):
                all_pieces_and_orientations.append(arad + spots)
            count += 1
        total = ''
        for w in range(len(pieces)):
            total += attrs[w]
        for i in range(11):
            for j in range(5):
                total += vector(i, j, 0).quickstr() + ' '
        total = total[:-1] + '\n'
        for p in all_pieces_and_orientations:
            s = attrs[p[0]]
            for pp in p[1:]:
                s += ' ' + pp.quickstr()
            print(s)
            total += s + '\n'
        total = total[:-1]
        f = open(file_loc, 'w')
        f.write(total)
        f.close()


def write_pyramid_perms(file_loc, pieces, w):
    attrs = [chr(ord('A') + i) for i in range(len(pieces))] + [
                vector(i, j, k).quickstr3()
                for k in range(5)
                for j in range(5 - k)
                for i in range(5 - k)
            ]
    with open(file_loc, 'w') as f:
        s = '\n'.join([' '.join(
            (attrs)
        )] + [
            ' '.join([attrs[num]] + [spot.quickstr3() for spot in spots])
            for num, piece in enumerate(pieces)
            for spots in piece.get_all_pyramid_spots(w)
        ])
        opts = [
            choice
            for lin in s.split('\n')[1:]
            for choice in lin.split(' ')
        ]
        s = '{} {} {}'.format(len(attrs), s.count('\n'), len(opts)) + '\n' + s
        f.write(s)




b = board(vector(0, 0, -.25), 11, 5, 11, 5)

pieces = []

# symmetry levels:
#     0: no symmetries
#     1: axial symmetry (only do first 2 orientations)
#     2: biaxial symmetry (only flip)
#     3: mirror image of itself (don't flip)
#     4: axial and mirror symmetry (only do first 2 orientations and don't flip)
#     5: entirely symmetric (don't do anything)

pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [3, 0]), symmetry_level=4))
pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [3, 0], [0, 1]), symmetry_level=0))
pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [0, 1]), symmetry_level=3))
pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [0, 1], [0, 2]), symmetry_level=3))
pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [0, 1], [1, 1]), symmetry_level=0))
pieces.append(piece(spots=([1, 0], [0, 1], [1, 1], [2, 1], [1, 2]), symmetry_level=5))
pieces.append(piece(spots=([0, 0], [1, 0], [1, 1], [2, 1], [2, 2]), symmetry_level=3))
pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [2, 1], [3, 1]), symmetry_level=0))
pieces.append(piece(spots=([0, 0], [1, 0], [1, 1]), symmetry_level=3))
pieces.append(piece(spots=([0, 0], [1, 0], [0, 1], [1, 1]), symmetry_level=5))
pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [0, 1], [2, 1]), symmetry_level=3))
pieces.append(piece(spots=([0, 0], [1, 0], [2, 0], [3, 0], [2, 1]), symmetry_level=0))


write_pyramid_perms('/users/claytonknittel/documents/xcode/math/math/kanoodle/pyrvert.txt', pieces, 5)

# b.write_permutations(pieces, 'kan.txt')


# d = dancing_links([['c', 'e'], ['a', 'd', 'g'], ['b', 'c', 'f'], ['a', 'd', 'f'], ['b', 'g'], ['d', 'e', 'g']],
#                   ['a', 'b', 'c', 'd', 'e', 'f', 'g'])

# d = dancing_links([['a'], ['b']], ['a', 'b'])

# print(d.solve())

