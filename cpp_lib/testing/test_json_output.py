"""
This file is used to test that the json outputs generated by the program (connections and sides)
are in a correct format, and are consistent.
to do so it load the images and put the side that in theory should match one on top of the other.
so it is easy for the user to visual confirm the result
"""

import cv2
import numpy as np
import json

def main():
    corners,connections = load_data()

    ids = connections.keys()

    black_image = np.zeros(shape=(5000,5000,3),dtype=np.uint8)

    for id in ids:

        for side in range(4):

            # reference to the piece that match the current piece
            match_ref = get_match_of_sides(connections[id],side)

            if(match_ref is None):
                continue

            other_id = match_ref["piece"]
            other_side = match_ref["side"]

            print(f"piece: {id} side: {side} match with piece: {other_id} side: {other_side}")

            #load the images
            this_img = load_image(id)
            other_img = load_image(other_id)

            temp = black_image.copy()
            temp[0:this_img.shape[0],0:this_img.shape[1],:] = this_img
            this_img = temp

            temp = black_image.copy()
            temp[0:other_img.shape[0],0:other_img.shape[1],:] = other_img
            other_img = temp

            # load the points
            this_p1,this_p2 = get_points_of_sides(corners[id],side)
            other_p1,other_p2 = get_points_of_sides(corners[other_id],other_side)

            print(f"this_p1: {this_p1}, this_p2: {this_p2}")
            print(f"other_p1: {other_p1}, other_p2: {other_p2}")

            this_img = rotate_and_center(this_img,this_p1,this_p2)
            other_img = rotate_and_center(other_img,other_p2,other_p1)


            result_image = black_image.copy()

            result_image[this_img[:,:,0] == 255,0] = 255
            result_image[other_img[:,:,2] == 255,2] = 255

            result_image = result_image[1000:-1000,1000:-1000,:]

            cv2.imshow("result_image",cv2.resize(result_image,(500,500)))
            cv2.waitKey(0)


# from the json point to the 
def get_points_of_sides(piece,side:int):
    key = f"side_{side}"

    p1 = piece[key]["p1"]
    p2 = piece[key]["p2"]

    return (p1["x"],p1["y"]),(p2["x"],p2["y"])


# from the json point to the 
def get_match_of_sides(piece,side:int):
    key = f"side_{side}"
    return piece[key]


def load_image(index: int):
    path = f"..//work_path//divided//{index}.jpeg"
    return cv2.imread(path)


def load_data():

    f = open(r"../work_path/results/corners.json")
    corners = json.load(f)
    f.close()

    corners_dict = {}
    for c in corners:
        corners_dict[c["piece_id"]] = c

    f = open(r"../work_path/results/connections_result.json")
    connections = json.load(f)
    f.close()

    connections_dict = {}
    for c in connections:
        connections_dict[c["piece_id"]] = c

    return corners_dict,connections_dict


def rotate_and_center(img, P1, P2):
    # Calculate the midpoint between P1 and P2
    midpoint = ((P1[0] + P2[0]) // 2, (P1[1] + P2[1]) // 2)

    # Get the dimensions of the image
    height, width = img.shape[:2]

    # Calculate the translation to center the midpoint
    tx = (width / 2) - midpoint[0]
    ty = (height / 2) - midpoint[1]

    # Define the translation matrix and perform the translation
    M = np.float32([[1, 0, tx], [0, 1, ty]])
    centered_img = cv2.warpAffine(img, M, (width, height))

    
    #Calculate the angle between P1 and P2
    angle = np.arctan2(P2[1] - P1[1], P2[0] - P1[0]) * (180 / np.pi)

    # Define the rotation matrix and perform the rotation
    M = cv2.getRotationMatrix2D((width/2,height/2), angle, 1)
    rotated_img = cv2.warpAffine(centered_img, M, (width, height))

    return rotated_img


if __name__ == "__main__":
    main()