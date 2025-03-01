/**
 * @file kmeans.h
 * @brief K-means clustering implementation for image color quantization.
 *
 * This file implements the k-means clustering algorithm for image processing:
 * - Color quantization through clustering
 * - Iterative refinement of color clusters
 * - Assignment of pixels to nearest cluster centers
 */

#ifndef KMeans_H
#define KMeans_H

#define cimg_display 0

#include "CImg.h"
using namespace cimg_library;

/**
 * @class KMeans
 * @brief K-means clustering implementation for image color quantization.
 *
 * KMeans provides color quantization through clustering:
 * - Reduces number of colors in an image
 * - Finds optimal color centers through iteration
 * - Assigns pixels to nearest color center
 * - Preserves dominant colors while reducing color count
 */
class KMeans
{
public:
    /**
     * @brief Constructor initializes k-means with image and cluster count.
     * @param image Source image to process
     * @param K Number of color clusters to create
     */
    KMeans(const CImg<unsigned char>& image, int K);

    /**
     * @brief Destructor.
     */
    ~KMeans();

    /**
     * @brief Trains the k-means model through iterative refinement.
     * @param iterations Number of refinement iterations to perform
     */
    void train(int iterations);

    /**
     * @brief Assigns pixels in target image to nearest cluster centers.
     * @param image Image to modify with clustered colors
     */
    void assign(CImg<unsigned char>& image) const;

private:
    /**
     * @brief Assigns pixels to new cluster centers based on distance.
     */
    void assignNewClusterCentres();

    /**
     * @brief Computes new centroids based on current cluster assignments.
     */
    void computeCentroids();

    /**
     * @brief Calculates Euclidean distance between pixel and cluster center.
     * @param clusterCenterIndex Index of cluster center
     * @param x X coordinate of pixel
     * @param y Y coordinate of pixel
     * @param z Color channel index
     * @return Euclidean distance
     */
    double euclideanDistance(int clusterCenterIndex, int x, int y, int z) const;

    /** @brief Source image data */
    CImg<unsigned char> m_image;
    /** @brief Cluster assignments for each pixel */
    CImg<unsigned char> m_labels;
    /** @brief Current cluster center colors */
    CImg<unsigned char> m_clusterCentres;

    /** @brief Number of clusters */
    int m_k;
    /** @brief Image width */
    int m_w;
    /** @brief Image height */
    int m_h;
};

#endif
