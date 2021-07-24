fdat = -1
with open("runtime/brain.txt") as bfile:
    fdat = bfile.read()


current_section = ""
read_line_into_arr = -1
neur_dat = {
    "inputs": [],
    "hidden": [],
    "outputs": [],
    "lines": [],
}
for line in fdat.split('\n'):
    line = line.strip()
    if line in ["inputs", "hidden", "outputs", "lines"]:
        current_section = line
        continue
    if current_section in ["inputs", "hidden", "outputs"]:
        if line == "###":
            read_line_into_arr = 1
            continue
        if read_line_into_arr == 1:
            lndat = line.split(',')
            neur_dat[current_section].append({
                'addr': int(lndat[0]),
                'func': int(lndat[1]),
                'role': int(lndat[2]),
                'actv': float(lndat[3]),
                'line': []
            })
            read_line_into_arr = 2
            continue
        if read_line_into_arr == 2:
            lndat = line.split(',')
            neur_dat[current_section][len(neur_dat[current_section]) - 1]["line"].append({
                'tddr': int(lndat[0]),
                'wght': float(lndat[1])
            })
            continue
    if current_section == "lines":
        lndat = line.split(',')
        try:
            neur_dat[current_section].append({
                'tddr': int(lndat[0]),
                'wght': float(lndat[1])
            })
        except:
            pass
print(neur_dat)