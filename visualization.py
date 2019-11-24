import matplotlib.pyplot as plt

with open('./data/cleanedDataset.txt', 'r') as fp:
    line = fp.readline()
    cnt = 1
    dataset = []
    while line:
        line = fp.readline().split()
        if(line):
            dataset.append(line)
        cnt += 1
    #testing for crime rates.
    yes_count = [0, 0]
    no_count = [0, 0]
    for rowData in dataset:
        if(rowData[-1] == 'r'):
            if(rowData[-4] == 'y'):
                yes_count[0] += 1
            else:
                no_count[0] += 1
        else:
            if(rowData[-4] == 'y'):
                yes_count[1] += 1
            else:
                no_count[1] += 1
    

    labels = ['r', 'd']
    plt.bar(labels, yes_count)

    plt.show()
