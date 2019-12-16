import os, sys, json, re

stats = {"total images": 0,
         "real goals": 0,
         "detected goals": 0,
         "true positives": 0,
         "true negatives": 0,
         "false positives": 0,
         "false negatives": 0}

threshold_score = 0.5

def main():
    if(len(sys.argv) != 3):
        print("Usage: ", sys.argv[0], "<real data directory> <theorical data directory>")
        return;
    real_path = sys.argv[1]
    if(real_path[-1] != '/'):
        real_path = real_path + '/'
    theorical_path = sys.argv[2]
    if(theorical_path[-1] != '/'):
        theorical_path = theorical_path + '/'
    real_dir = os.listdir(real_path)
    theorical_dir = os.listdir(theorical_path)

    for file in real_dir:
       with open(real_path+file, "r") as f:
           data = json.load(f)
           for key in data:
               check(data[key], theorical_path)
    display_stats(stats)


def check(real_data, theorical_path):
    filename = real_data['filename']
    with open(theorical_path+filename+'.txt', "r") as f:
        th_boxes = extract_boxes(f.read())
        re_box = outline(real_data['regions'])
        stats["total images"] += 1
        compare(re_box, th_boxes)

def extract_boxes(str):
    boxes = []
    lines = str.splitlines()
    for line in lines:
        start = line.find('[') + 1
        end = line.find(']', start)
        coord_str = line[start:end].split(' ')
        coord = []
        for c in coord_str:
            if c != '':
                coord.append(int(c))
        boxes.append(coord)
    if(boxes == [] or boxes == [[]]):
        boxes = [[0,0,0,0]]
    return boxes

def outline(region):
    if(len(region) != 0):
        return [
        min(region[0]["shape_attributes"]["all_points_y"]),
        min(region[0]["shape_attributes"]["all_points_x"]),
        max(region[0]["shape_attributes"]["all_points_y"]),
        max(region[0]["shape_attributes"]["all_points_x"])]
    return [0,0,0,0]

def compare(re_box, th_boxes):
    if(re_box == [0,0,0,0]):
        if(th_boxes == [[0,0,0,0]]):
            stats["true negatives"] += 1
        else:
            stats["false positives"] += len(th_boxes)
            stats["detected goals"] += len(th_boxes)
    else :
        stats["real goals"] += 1
        if(th_boxes == [[0,0,0,0]]):
            stats["false negatives"] += 1
        else:
            stats["detected goals"] += len(th_boxes)
            for th_box in th_boxes:
                score = inter(re_box, th_box)/union(re_box, th_box)
                if(score > threshold_score):
                    stats["true positives"] += 1
                else:
                    stats["false positives"] += 1

def inter(re_box, th_box):
    y1 = max(re_box[0], th_box[0])
    x1 = max(re_box[1], th_box[1])
    y2 = min(re_box[2], th_box[2])
    x2 = min(re_box[3], th_box[3])
    return area([y1, x1, y2, x2])

def union(re_box, th_box):
    return area(re_box)+area(th_box)-inter(re_box, th_box)

def area(box):
    if(box[0] < box[2] and box[1] < box[3]):
        return (box[2]-box[0])*(box[3]-box[1])
    return 0

def display_stats(stats):
    print("Total images:", stats["total images"])
    print("True positives:", round(100*stats["true positives"]/stats["real goals"], 2), "%")
    print("True negatives:", round(100*stats["true negatives"]/(stats["total images"] - stats["real goals"]), 2), "%")
    print("False positives:", round(100*stats["false positives"]/stats["real goals"], 2), "%")
    print("False negatives:", round(100*stats["false negatives"]/(stats["total images"] - stats["real goals"]), 2), "%")

main()
