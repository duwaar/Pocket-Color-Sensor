
with open('color_files.txt', 'r') as file:
    color_files = file.read().split()

variable_names = []
for color_file in color_files:
    with open(color_file, 'r') as color_data:
        color_data.readline() # First line is labels, not data.
        red_sum   = 0
        green_sum = 0
        blue_sum  = 0
        line = color_data.readline()
        count = 0
        while line != '':
            columns = line.strip().split(sep=',')
            red_sum   += int(columns[1])
            green_sum += int(columns[2])
            blue_sum  += int(columns[3])
            line = color_data.readline()
            count += 1

    red_avg   = round(red_sum   / count)
    green_avg = round(green_sum / count)
    blue_avg  = round(blue_sum  / count)

    
    color_name = color_file[0:(color_file.index('.') - 2)].upper()
    variable_name = 'VEC_' + color_name
    variable_names.append(variable_name)
    print(f'Color {variable_name}({red_avg}, {green_avg}, {blue_avg}, \"{color_name}\");')
    
print()
print('Color color_list[] = {')
for name in variable_names:
    print(f'    {name}', end='')
    if variable_names.index(name) < len(variable_names) - 1:
        print(',')
    else:
        print()
print('};')

