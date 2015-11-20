def prefix(name):
    index = name.rfind('/')
    if index == -1:
        return ""
    else:
        return name[:index]

def suffix(name):
    index = name.rfind('/')
    if index == -1:
        return name
    else:
        return name[index+1:]

