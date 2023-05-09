
#include <iostream>
#include "../solving/graphic_piece/Side.h"
#include "../solving/graphic_piece/PieceShape.h"
#include "../solving/graphic_piece/util_piece.h"
#include "../solving/logic_piece/PieceConnection.h"
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
#include "../solving/groped_pieces/GroupedPieces.h"
#include "../solving/puzzle_preview/PieceArray.h"
#include <time.h>
#include "../solving/groped_pieces/grouped_pieces_errors.h"
#include "../solving/groped_pieces/GroupedPiecesHolder.h"


void test_piece_array(){

    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];

    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    imshow("AAAAAAA",pieces_images->get_image());

    PieceArray<PreviewHolder> pa = PieceArray<PreviewHolder>();

    PreviewHolder base = PreviewHolder(&pieces_images[4], 0);
    pa.set(0,0,std::move(base));


    //imshow("puzzle", pa.get_preview_image());waitKey(0);

    pa.grow_x();

    base = PreviewHolder(&pieces_images[5], 3);pa.set(1, 0, std::move(base));


    //imshow("puzzle", pa.get_preview_image());waitKey(0);

    pa.grow_y();

    base = PreviewHolder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));


    //imshow("puzzle", pa.get_preview_image());waitKey(0);


    //imshow("puzzle", pa.get_preview_image());waitKey(0);

    base = PreviewHolder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));


    PieceArray pa2 = pa;


    pa.attach_bottom(pa2);
    pa.attach_bottom(pa2);

    PieceArray pa3 = pa;

    pa.attach_right(pa3);


    imshow("puzzle", pa.get_preview_image());waitKey(0);
}

void test_grouped_piece_2_constructor(){
    string path = "../../dataset/test_2x3/connections";

    PieceConnection::set_number_of_pieces(6);
    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceConnection pieces[6];
    PieceImage pieces_images[6];


    // load all the pieces
    for(int i=0; i<6; i++){
        pieces[i].became(path,i);
        pieces_images[i] = PieceImage(i);
    }

    GroupedPieces<1> top_left = GroupedPieces<1>(&pieces[4]);
    GroupedPieces<1> top_right = GroupedPieces<1>(&pieces[5]);
    top_right.rotate_by(3);
    GroupedPieces<1> bottom_left = GroupedPieces<1>(&pieces[3]);
    bottom_left.rotate_by(3);
    GroupedPieces<1> bottom_right = GroupedPieces<1>(&pieces[2]);

    GroupedPieces<2> sq =  GroupedPieces<2>(&top_left,&top_right,&bottom_right,&bottom_left);

    auto pa = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa.get_preview_image());waitKey(0);
    imshow("p",pa.get_preview_image());waitKey(0);

    sq.rotate_by(1);

    auto pa2 = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa2.get_preview_image());waitKey(0);
    imshow("p",pa2.get_preview_image());waitKey(0);
}


void test_piece_array_shore(){

    Mat image_og;
    Mat resized;

    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];


    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    PieceArray<ShoringHolder> pa = PieceArray<ShoringHolder>();

    ShoringHolder base = ShoringHolder(&pieces_images[4], 0);
    base.rotate_by(-0.1);
    pa.set(0,0,std::move(base));



    image_og = pa.get_preview_image();
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);

    pa.grow_x();

    base = ShoringHolder(&pieces_images[5], 3);pa.set(1, 0, std::move(base));


    image_og = pa.get_preview_image();
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);


    pa.grow_y();

    base = ShoringHolder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));

    base = ShoringHolder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));
    image_og = pa.get_preview_image();

    cout << "shore: " << pa.get_shore() << endl;

    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);

    PieceArray pa2 = pa;


    pa.attach_bottom(pa2);
    //pa.attach_bottom(pa2);

    //PieceArray pa3 = pa;

    //pa.attach_right(pa3);


    cout << "shore: " << pa.get_shore() << endl;

    image_og = pa.get_preview_image();
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);
}

void test_grouped_piece_holder(){
    string path = "../../dataset/test_2x3/connections";
    PieceConnection::set_number_of_pieces(6);
    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceConnection pieces[6];
    PieceImage pieces_images[6];


    // load all the pieces
    for(int i=0; i<6; i++){
        pieces[i].became(path,i);
        pieces_images[i] = PieceImage(i);
    }

    list<GroupedPieces<1>> list1 = {};

    for(int i=0; i<6; i++){
        list1.emplace_back(pieces+i);
    }

    auto group_1 = GroupedPiecesHolder<1>(list1);

    GroupedPieces<2> sq =  GroupedPieces<2>(
            &group_1.get(4,0),
            &group_1.get(5,3),
            &group_1.get(2,0),
            &group_1.get(3,3)
    );

    auto pa = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa.get_preview_image());waitKey(0);

    sq.rotate_by(1);

    auto pa2 = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa2.get_preview_image());waitKey(0);
}

void test_piece_array_save(){

    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];

    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    PieceArray<PreviewHolder> pa = PieceArray<PreviewHolder>();

    PreviewHolder base = PreviewHolder(&pieces_images[4], 0);
    pa.set(0,0,std::move(base));
    pa.grow_x();

    base = PreviewHolder(&pieces_images[5], 3);
    pa.set(1, 0, std::move(base));
    pa.grow_y();

    base = PreviewHolder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));


    base = PreviewHolder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));

    pa.save_as_file("../save.bin");

    PieceArray<PreviewHolder> new_pa = PieceArray<PreviewHolder>();

    new_pa.load_from_file("../save.bin",pieces_images);

    imshow("original",pa.get_preview_image());
    imshow("after save",new_pa.get_preview_image());

    waitKey(0);


}