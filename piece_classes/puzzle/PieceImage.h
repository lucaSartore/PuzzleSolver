//
// Created by luca on 4/25/23.
//

#ifndef PIECECLASS_PIECEIMAGE_H
#define PIECECLASS_PIECEIMAGE_H

#include <opencv2/opencv.hpp>
#include <iostream>


class PieceImage {
private:
    // the original piece
    cv::Mat piece;
    // a resized and lined up version of the piece to use in preview
    cv::Mat piece_resized;
    // coordinates of the corners
    cv::Point points[4];
    // coordinates of the corners in the resized image
    cv::Point points_resized[4];
    // path form where pieces are generated from
    static std::string origin_path;
    // the id of the current piece
    int id;
public:
    /// default constructor
    PieceImage() = default;
    /// disable copy constructor, since all the piece will be the same in all the program it make mor sense to use pointers
    PieceImage(PieceImage& other) = delete;
    /// crete a piece with the given id from the origin path that has been chosen with `set_origin_path`
    explicit PieceImage(int piece_id);
    /// create a piece with the given id, form the specified path
    PieceImage(int piece_id, std::string path);
    /// set the origin path that will be used by the one parameter constructor
    static void set_origin_path(std::string path);
    /// return the original image
    cv::Mat get_image();
    /// return the resized image for the preview original image
    cv::Mat get_image_resized();
    /// return one of the 4 point, based on the index
    cv::Point get_point(int index);
    /// return one of the 4 resized point, based on the index
    cv::Point get_point_resized(int index);
    /// destructor
    ~PieceImage()  = default;
    /// return the x resolution of the preview image
    int get_preview_x_res();
    /// return the y resolution of the preview image
    int get_preview_y_res();
};



#endif //PIECECLASS_PIECEIMAGE_H
