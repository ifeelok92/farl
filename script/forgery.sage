from math import *

def slope(b):
    return 1/(b-1)*log(((math.pi*b)**(1./b)*b/(2*math.pi*math.e)),2)


# NewHope Cost Model
def svp_plausible(b):
	return b *log(sqrt(4./3))/log(2)  # .2075 * b 

def svp_quantum(b):
	return b *log(sqrt(13./9))/log(2)  # .265 * b  

def svp_classical(b):
	return b * log(sqrt(3./2))/log(2)   # .292 * b 
    

# Frodo Cost Model
def svp_plausible_l(b):
	return b *log(sqrt(4./3))/log(2) +log(b,2)   

def svp_quantum_l(b):
	return b *log(sqrt(13./9))/log(2) +log(b,2)  

def svp_classical_l(b):
	return b * log(sqrt(3./2))/log(2)  +log(b,2)      

def rootHF(b):
    return 2 ** (0.5 * slope(b))
    
# def num_of_q(n, r1):
#     return n - r1
    
def dim_of_extracted(n, r1, r2):
    return 2 * n - r1 - r2


def term_of_alpha(j, b, first_term, r1):
    return first_term + slope(b) * (j - r1)



def forgery_cost_3n(n, q, a, fun=svp_quantum):
    best_cost = 9999
    best_b = 9999
    w = 3*n
    print "dim=", w
    # w = 3*n
    best_r2 = 9999
        # forgetting q-vector, then r1=0
    for b in range(200, w):
        r2 = int(floor(-0.5 + sqrt(.25 + 2.0 * n * log(q, 2) / slope(b)) + 1))
        l1 = slope(b) * (r2 - 1)
        # l1 = n * log(q, 2) / (r2 - 1) + 0.5 * (r2 - 2) * slope(b)
        if r2 - 1 > w: r2 = w + 1
        std = 2 ** l1 / sqrt(r2 - 1)
        log_p = log(erf(a / (sqrt(2) * std)), 2)
        cost = max(fun(b), fun(b) - (r2 - 1) * log_p - b*log(sqrt(4./3), 2))
        if cost < best_cost:
            best_cost = cost
            best_b = b
            best_r2 = r2
    return best_b, best_r2, best_cost

def dilithium_cost_cr(n, q, a, fun=svp_quantum):
    best_cost = 9999
    best_b = 9999
    best_w = 9999
    best_r2 = 9999
    w=2*n+1
    # for w in range(2*n+1, 2 * n + 2):
    # forgetting q-vector, then r1=0
    for b in range(200, w):
        r2 = int(floor(-0.5 + sqrt(.25 + 2 * n * log(q,2) / slope(b)) + 1))
        l1 = slope(b) * (r2 - 1)
        # l1 = n * log(q, 2) / (r2 - 1) + 0.5 * (r2 - 2) * slope(b)
        if r2 - 1 > w: r2 = w + 1
        std = 2 ** l1 / sqrt(r2 - 1)
        log_p = log(erf(a / (sqrt(2) * std)), 2)
        cost = max(fun(b), fun(b) - (r2 - 1) * log_p - b*log(sqrt(4./3), 2))
        if cost < best_cost:
            best_cost = cost
            best_b = b
            # best_w = w
            best_r2 = r2
    return best_b, best_r2, best_cost


# iteration probability

def sucPro1(q, alpha, l, p, n):
    pr = exp((-(2*l+1)/alpha+(-2*p*l+1)/q)*n)
    return pr



#q20=[964609,974849,995329,1017857,1032193,1038337]
q20=[1038337]

# (q /2-p(l-1))/p

 
n = 1024
l = 44
d = 256
p = 8
eta1 = []
eta2 = []
for i in range(len(q20)):
    eta1.append(0.5 * q20[i] / p - l + 1 )
    alpha=floor(q20[i]/16)
    eta2.append(0.5*alpha-l)



classical = []
quantum = []
plausible=[]

for i in range(len(q20)):
    c = eta2[i] / eta1[i]
    q=c*q20[i]
    a = 2*eta2[i]
    print "Signature Forgery: dim = 3n"
    print "------------------------------"
    print
    print
    print "---------classical-------------"
    b, r2, cost = forgery_cost_3n(n, q, a,svp_classical_l)
    classical.append(cost)
    print "b\tr2\tsecurity"
    print b, " \t& ", r2," \t& ",  cost
    print "---------quantum-------------"
    b, r2, cost = forgery_cost_3n(n, q, a, svp_quantum_l)
    quantum.append(cost)
    print "b\tr2\tsecurity"
    print b, " \t& ", r2," \t& ",  cost
    # print "---------plausible-------------"
    # b,  r2, cost = forgery_cost_3n(n, q, a, svp_plausible)
    # plausible.append(cost)
    # print "b\tr2\tsecurity"
    # print b, "\t& ", r2, " \t& ", cost
    print "--------------------------------"
    print



  
for i in range(len(q20)):
    c = eta2[i] / eta1[i]
    q=c*q20[i]
    a = eta2[i]
    print "Signature Forgery: dim = 2n + 1"
    print "++++++++++++++++++++++++++++++++"
    print
    print
    print "---------classical-------------"
    b, r2, cost = dilithium_cost_cr(n, q, a,svp_classical_l)
    classical.append(cost)
    print "b\tr2\tsecurity"
    print b, " \t& ", r2," \t& ",  cost
    print "---------quantum-------------"
    b, r2, cost = dilithium_cost_cr(n, q, a, svp_quantum_l)
    quantum.append(cost)
    print "b\tr2\tsecurity"
    print b, " \t& ", r2," \t& ",  cost
    # print "---------plausible-------------"
    # b,  r2, cost = dilithium_cost_cr(n, q, a, svp_plausible)
    # plausible.append(cost)
    # print "b\tr2\tsecurity"
    # print b, "\t& ", r2, " \t& ", cost
    print "++++++++++++++++++++++++++++++++++"
    print

