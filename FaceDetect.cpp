#include <opencv2/opencv.hpp>
#include <iostream>
#include "FaceDetect.h"
#include <chrono>

using namespace cv;
using namespace std;

bool FaceDetect::SecurityCheck(uint64_t time)
{
    // Load the face cascades
    CascadeClassifier face_cascade;
    face_cascade.load("haarcascade_frontalface_default.xml");

    vector<Mat> ref_images;
    Mat ref_image1 = imread("reference_face1.jpg", IMREAD_GRAYSCALE);
    Mat ref_image2 = imread("reference_face2.jpg", IMREAD_GRAYSCALE);
    Mat ref_image3 = imread("reference_face3.jpg", IMREAD_GRAYSCALE);
    Mat ref_image4 = imread("reference_face4.jpg", IMREAD_GRAYSCALE);
    Mat ref_image5 = imread("reference_face5.jpg", IMREAD_GRAYSCALE);
    Mat ref_image6 = imread("reference_face6.jpg", IMREAD_GRAYSCALE);
    Mat ref_image7 = imread("reference_face7.jpg", IMREAD_GRAYSCALE);

    // Resize all reference images to the size of the first reference image
    resize(ref_image2, ref_image2, ref_image1.size());
    resize(ref_image3, ref_image3, ref_image1.size());
    resize(ref_image4, ref_image4, ref_image1.size());
    resize(ref_image5, ref_image5, ref_image1.size());
    resize(ref_image6, ref_image6, ref_image1.size());
    resize(ref_image7, ref_image7, ref_image1.size());

    ref_images.push_back(ref_image1);
    ref_images.push_back(ref_image2);
    ref_images.push_back(ref_image3);
    ref_images.push_back(ref_image4);
    ref_images.push_back(ref_image5);
    ref_images.push_back(ref_image6);
    ref_images.push_back(ref_image7);

    // Set up the video capture
    VideoCapture capture(0);
    if (!capture.isOpened())
    {
        cout << "Error: Could not open the camera." << endl;
        return -1;
    }

    // Capture the video frame by frame
    Mat frame;
    while (capture.read(frame))
    {
        // Convert the frame to grayscale
        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

        // Detect faces in the frame
        vector<Rect> faces;
        face_cascade.detectMultiScale(frame_gray, faces);

        // Loop through the detected faces
        for (size_t i = 0; i < faces.size(); i++)
        {
            // Crop the face from the frame
            Mat face = frame_gray(faces[i]);

            // Resize the face to the same size as the reference image
            Mat face_resized;
            resize(face, face_resized, ref_images[0].size());

            // Loop through the reference images
            for (const Mat& ref_image : ref_images)
            {
                // Calculate the L2 distance between the face and the reference image
                double distance = norm(face_resized, ref_image, NORM_L2);

                // If the distance is below a certain threshold, it is a match
                cout << "Face Similarity Score (lower is better): " << distance << endl;
                if (distance < 300)
                {
                cout << "Match found!" << endl;
                return true;
                }
            }
        }

        // Display the frame
        imshow("Frame", frame);

        // Break the loop if the user presses the 'q' key
        if (waitKey(10) == 'q')
            break;

        uint64_t compareTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (compareTime >= time + 11000)
        {
            return false;
        }
    }

    return false;
}

//ToDo
//1. Add vector list of reference images
//2. Add conversion from decrypted hex to decrypted plaintext