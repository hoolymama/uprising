#ifndef KMeans_H
#define KMeans_H

#define cimg_display 0

#include "CImg.h"
using namespace cimg_library;

class KMeans
{

public:
    KMeans(const CImg<unsigned char> &image, int K);

    ~KMeans();
    void train(int iterations);
    // void convert();
    void assign(CImg<unsigned char> &image) const;

private:

    void assignNewClusterCentres();
    void computeCentroids();
    double euclideanDistance(int clusterCenterIndex, int x, int y, int z) const;

 

    CImg<unsigned char> m_image;
    CImg<unsigned char> m_labels;
    CImg<unsigned char> m_clusterCentres;

    int m_k;
    int m_w;
    int m_h;
};

#endif
