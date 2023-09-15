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
#include "../solving/puzzle_preview/PreviewManager.h"
#include <time.h>
#include "../solving/groped_pieces/grouped_pieces_errors.h"
#include "../solving/groped_pieces/GroupedPiecesHolder.h"
#include "../communication/communication_image.h"
#include "../rust_solving/src/PieceArrayDLL/piece_array_dll.h"
#include "../solving/calculate_connections.h"
using namespace std;
void test_rust_integration(){

    load_images_to_piece_array_wrapper("../../../dataset/test_2x3/divided/");

    auto a = create_piece_array_wrapper(1,1, nullptr);

    //generate_test_image(a);

    free_images_of_piece_array_wrapper();
}

void test_piece_array(){

    PieceImage::set_origin_path("../../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];

    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    PieceArray pa = PieceArray();

    Holder base = Holder(&pieces_images[4], 0);
    pa.set(0,0,std::move(base));


    Mat resized;
    cv::resize(pa.get_image(SHORING),resized, Size(1000,1000));
    imshow("puzzle", resized);waitKey(0);


    pa.grow_x();

    base = Holder(&pieces_images[5], 3);pa.set(1, 0, std::move(base));


    cv::resize(pa.get_image(SHORING),resized, Size(1000,1000));
    imshow("puzzle", resized);waitKey(0);

    pa.grow_y();

    base = Holder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));


    base = Holder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));


    cv::resize(pa.get_image(SHORING),resized, Size(1000,1000));
    imshow("puzzle", resized);waitKey(0);

    PieceArray pa2 = pa;


    pa.attach_bottom(pa2);
    pa.attach_bottom(pa2);

    PieceArray pa3 = pa;

    pa.attach_right(pa3);


    cv::resize(pa.get_image(SHORING),resized, Size(1000,1000));
    imshow("puzzle", resized);waitKey(0);
}

void test_grouped_piece_2_constructor(){
    string path = "../../../dataset/test_2x3/connections";

    PieceConnection::set_number_of_pieces(6);
    PieceImage::set_origin_path("../../../dataset/test_2x3/divided");
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

    auto pa = sq.get_piece_array(pieces_images);
    imshow("p",pa.get_image(PREVIEW));waitKey(0);
    imshow("p",pa.get_image(PREVIEW));waitKey(0);

    sq.rotate_by(1);

    auto pa2 = sq.get_piece_array(pieces_images);
    imshow("p",pa2.get_image(PREVIEW));waitKey(0);
    imshow("p",pa2.get_image(PREVIEW));waitKey(0);
}


void test_piece_array_shore(){

    Mat image_og;
    Mat resized;

    PieceImage::set_origin_path("../../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];


    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    PieceArray pa = PieceArray();

    Holder base = Holder(&pieces_images[4], 0);
    //base.rotate_by(-0.1);
    pa.set(0,0,std::move(base));



    image_og = pa.get_image(PREVIEW);
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);

    pa.grow_x();

    base = Holder(&pieces_images[5], 3);pa.set(1, 0, std::move(base));


    image_og = pa.get_image(PREVIEW);
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);


    pa.grow_y();

    base = Holder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));

    base = Holder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));
    image_og = pa.get_image(PREVIEW);

    cout << "shore: " << pa.get_shore() << endl;

    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);

    PieceArray pa2 = pa;


    pa.attach_bottom(pa2);
    //pa.attach_bottom(pa2);

    //PieceArray pa3 = pa;

    //pa.attach_right(pa3);


    cout << "shore: " << pa.get_shore() << endl;

    image_og = pa.get_image(PREVIEW);
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);
}

void test_grouped_piece_holder(){
    string path = "../../../dataset/test_2x3/connections";
    PieceConnection::set_number_of_pieces(6);
    PieceImage::set_origin_path("../../../dataset/test_2x3/divided");
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

    auto pa = sq.get_piece_array(pieces_images);
    imshow("p",pa.get_image(PREVIEW));waitKey(0);

    sq.rotate_by(1);

    auto pa2 = sq.get_piece_array(pieces_images);
    imshow("p",pa2.get_image(PREVIEW));waitKey(0);
}

void test_piece_array_save(){

    PieceImage::set_origin_path("../../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];

    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    PieceArray pa = PieceArray();

    Holder base = Holder(&pieces_images[4], 0);
    pa.set(0,0,std::move(base));
    pa.grow_x();

    base = Holder(&pieces_images[5], 3);
    pa.set(1, 0, std::move(base));
    pa.grow_y();

    base = Holder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));


    base = Holder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));

    pa.save_as_file("../../save.bin");

    PieceArray new_pa = PieceArray();

    new_pa.load_from_file("../../save.bin",pieces_images);

    imshow("original",pa.get_image(PREVIEW));
    imshow("after save",new_pa.get_image(PREVIEW));

    waitKey(0);
}


int test_index = 0;
std::mutex mutex_test_index;

void image_generator_thread(int thread_id){
    cout << "thread " << thread_id << " started" << endl;
    srand(time(nullptr));
    while (true){
        // create an image with a random color
        Mat image = Mat::zeros(Size(500,500),CV_8UC3);
        int r = rand()%255;
        int g = rand()%255;
        int b = rand()%255;

        mutex_test_index.lock();
        int n = test_index;
        test_index++;
        if(n>50){
            break;
        }
        mutex_test_index.unlock();

        Scalar random_color = Scalar(b,g,r);
        floodFill(image,Point(0,0),random_color);
        putText(image,to_string(n),Point(100,400),0,10,Scalar(0,0,0),40);
        cout << "thread " << thread_id << " ready to write image" << endl;
        PreviewManager::output_preview_image(image);
        cout << "thread " << thread_id << " has written an image" << endl;
        //return;
    }
};

void test_preview_manager(){
    const int N_THREADS = 6;

    // enable the preview
    PreviewManager::enable_preview();

    // spawn some threads
    thread threads[N_THREADS];
    for(int i=0; i<N_THREADS; i++){
        threads[i] = thread(image_generator_thread,i);
    }

    this_thread::sleep_for(chrono::duration<int,milli>(1000));

    while (true) {
        //cout << "requesting an image" << endl;
        PreviewManager::next_preview_image();
        //cout << "got an image" << endl;
        Mat image = PreviewManager::get_image();
        imshow("image", image);
        waitKey(0);
        //break;
    }

    threads[0].join();
}

void test_image_ram_encode(){
    Mat image = Mat::zeros(Size(1000,1000),CV_8UC3);
    floodFill(image,Point(0,0),Scalar(100,200, 222));
    rectangle(image,Point(10,10),Point(200,700),Scalar(255,40,40),-1);

    auto image_ram = store_image_to_ram(image);

    Mat new_image = load_image_from_ram(image_ram.get_image_pointer());

    imshow("original",image);
    imshow("saved", new_image);
    waitKey(0);
}

void test_piece_array_size_constructor(){

    PieceArray pa =  PieceArray(2,2);

    PieceImage::set_origin_path("../../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];

    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }


    Holder base = Holder(&pieces_images[4], 0);
    pa.set(0,0,std::move(base));

    base = Holder(&pieces_images[5], 3);
    pa.set(1, 0, std::move(base));

    base = Holder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));

    base = Holder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));

    imshow("image",pa.get_image(PREVIEW));

    waitKey(0);
}


void test_comparing(){

    cout << "aaa" << endl;
    PieceShape::set_origin_path("../../../../dataset/test_4x4_digital/divided");

    PieceShape p0 = PieceShape(0);
    PieceShape p1 = PieceShape(1);
    PieceShape p6 = PieceShape(6);

    //p0.get_side(1).compare_to(p1.get_side(3), true);

    //p0.get_side(2).compare_to(p6.get_side(0), true);

    calculate_all_connections(
            "../../../../dataset/test_16x16_digital/divided",
            "../../../../dataset/test_16x16_digital/connections",
            256
            );
}