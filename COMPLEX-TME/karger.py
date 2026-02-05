import random, copy, time

#on représente un multigraphe par une matrice d'ajacence où chaque case contient le nombre d'arêtes entre 2 sommets
adjacency_matrix = [[0 for y in range (3)] for x in range (3)]

def contraction(G, areteBegin, areteEnd):
    if G[areteBegin][areteEnd] < 1 :
        print('error')
        return G

    #faire entrer les aretes du 2e sommet dans le premier (la matrice est symmetrique)
    for i in range(len(G)):
        G[areteBegin][i] += G[areteEnd][i]

        if (i != areteBegin):
            G[i][areteBegin] += G[i][areteEnd]

    #suppression de loop
    G[areteBegin][areteBegin] = 0

    #supprimer le 2e sommet
    del(G[areteEnd])

    for i in range(len(G)):
        del(G[i][areteEnd])

    return G

cycle5 = [
    [0,1,0,0,1],  # 0 connecté à 1 et 4
    [1,0,1,0,0],  # 1 connecté à 0 et 2
    [0,1,0,1,0],  # 2 connecté à 1 et 3
    [0,0,1,0,1],  # 3 connecté à 2 et 4
    [1,0,0,1,0],  # 4 connecté à 0 et 3
]

complet5 = [
    [0,1,1,1,1],
    [1,0,1,1,1],
    [1,1,0,1,1],
    [1,1,1,0,1],
    [1,1,1,1,0],
]

# matrice d'adjacence du graphe complet à n sommets
n = 400
completN = [[0 if i == j else 1 for j in range(n)] for i in range(n)]

#La contraction d'une arête est en O(2n) car on itère 2 fois sur les sommets dans la matrice d'adjacence.

def pick_random_edge(G):
    #le nombre est 2 fois supérieur au nb réel d'arêtes car la matrice est symétrique mais c'est fine
    nb_edges = (sum(sum(row) for row in G))
    index = random.randrange(0, nb_edges)

    for i in range(len(G)):
        for j in range (len(G)):
            if G[i][j] == 0: continue

            index -= G[i][j]
            if (index <= 0): return (min(i,j), max(i,j))

    print('Error')
    return (0,0)

def karger(G):
    nb_vertices = len(G)
    vertex_list = [set([i]) for i in range (nb_vertices)]

    while(nb_vertices > 2):
        (i,j) = pick_random_edge(G)
        G = contraction(G, i, j)

        vertex_list[i] = vertex_list[i] | vertex_list[j] #union de 2 sets
        vertex_list[j] = set() #création de set vide

        nb_vertices = len(G)

    for item in vertex_list:
        if len(item) > 0:
            return item

def karger_iterated(G, T):
    graphe_de_test = copy.deepcopy(G)
    final_result = karger(graphe_de_test)

    for i in range(T-1):
        graphe_de_test = copy.deepcopy(G)
        temp_result = karger(graphe_de_test)
        
        #On fait une disjonction de cas ici car si on a une coupe avec 1 sommet d'un côté et n-1 de l'autre, on peut retourner n'importe lequel des 2 ensembles.

        if (len(temp_result) < len(final_result) and len(temp_result) < len(G)/2):
            final_result = temp_result
        
        if (len(temp_result) > len(final_result) and len(temp_result) > len(G)/2):
            final_result = temp_result

    return final_result

# On veut checker le temps d'exécution et la qualité de la solution en fonction du nombre 
# d'itérations afin de déterminer à partir de combien d'itérations il n'est pas très intéressant de continuer.
# NB: T doit être un multiple de 4, sinon ça casse (un peu) la fonction de test.
def testKargerIterated(T):
    for i in range(int(T/4)):
        graphe_de_test = copy.deepcopy(completN)
        nbIter = i*4
        
        timea = time.perf_counter()
        result = karger_iterated(graphe_de_test, nbIter)
        timeb = time.perf_counter()
        
        duration = timeb - timea
        nbSommetsDansPetitEnsemble = min(len(result), len(graphe_de_test) - len(result))

        print(f"T = {nbIter} - Exécution en {duration:.2f}s. Petit ensemble contenant {nbSommetsDansPetitEnsemble} sommets.")

#Douteux
def kargerSteinIntermediaire(G, T):
    nb_vertices = len(G)
    vertex_list = [set([i]) for i in range (nb_vertices)]

    while(nb_vertices > T):
        (i,j) = pick_random_edge(G)
        G = contraction(G, i, j)

        vertex_list[i] = vertex_list[i] | vertex_list[j] #union de 2 sets
        vertex_list[j] = set() #création de set vide

        nb_vertices = len(G)

    for item in vertex_list:
        if len(item) > 0:
            return item