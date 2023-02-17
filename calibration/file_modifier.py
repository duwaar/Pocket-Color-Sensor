

while True:
    command = input('file to modify: ')
    if command == 'exit':
        break
    else:
        file_name = command

    output = 'num,red,green,blue\n'
    with open(file_name, 'r') as file:
        file.readline() # The first line is garbage, so ignore it.
        line = file.readline() 
        i = 0
        while line:
            output += str(i) + ',' + line
            line = file.readline()
            i += 1

    file_name_parts = file_name.split(sep='.')
    new_file_name = file_name_parts[0] + '_1.' + file_name_parts[1]
    with open(new_file_name, 'w') as file:
        file.write(output)

    print('Done!')
