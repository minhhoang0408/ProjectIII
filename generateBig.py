import sys

def main():
    if len(sys.argv)!=2 :
        print('need one, and only one argument')
        sys.exit()
    n = int(sys.argv[1])
    print("Big %s" % n)
    print("0,0")
    print("%s,%s" % (1+n*3,1+n*3))
    print(n*n)
    for i in range(n):
        for j in range(n):
            I=(i*3)+1
            J=(j*3)+1
            print("4 %s,%s %s,%s %s,%s %s,%s" % (I,J,I,J+2,I+2,J+2,I+2,J))

if __name__=="__main__" :
    main()
