
#include <random>
#include "kmeans.h"

KMeans::KMeans(const CImg<unsigned char> &image, int K) : m_k(K)
{
    m_image.assign(image, true); // shared data (it's const)
    m_w = image.width();
    m_h = image.height();
    m_clusterCentres.assign(m_k, 1, 1, 3);
    m_labels.assign(m_w, m_h, 1, 1, 0);
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distribx(0, m_w - 1);
    std::uniform_int_distribution<> distriby(0, m_h - 1);

    for (int i = 0; i < m_k; i++)
    {
        int randX = distribx(gen);
        int randY = distriby(gen);

        m_clusterCentres(i, 0, 0, 0) = m_image(randX, randY, 0);
        m_clusterCentres(i, 0, 0, 1) = m_image(randX, randY, 1);
        m_clusterCentres(i, 0, 0, 2) = m_image(randX, randY, 2);
    }
    assignNewClusterCentres();
}

KMeans::~KMeans()
{
}

void KMeans::train(int iterations)
{
 
    for (int i = 0; i < iterations; i++)
    {
        computeCentroids();
        assignNewClusterCentres();

    }
}
 

void KMeans::assign(CImg<unsigned char> &image) const
{
    image.assign(m_w, m_h, 1, 3);
    for (int y = 0; y < m_h; y++)
    {
        for (int x = 0; x < m_w; x++)
        {
            int label = m_labels(x, y);
            image(x, y, 0) = m_clusterCentres(label, 0, 0, 0);
            image(x, y, 1) = m_clusterCentres(label, 0, 0, 1);
            image(x, y, 2) = m_clusterCentres(label, 0, 0, 2);
        }
    }
}


void KMeans::assignNewClusterCentres()
{
    for (int y = 0; y < m_h; y++)
    {
        for (int x = 0; x < m_w; x++)
        {
            int centroidLabel = 0;

            unsigned char r = m_image(x, y, 0);
            unsigned char g = m_image(x, y, 1);
            unsigned char b = m_image(x, y, 2);

            double distance, min_dist;
            min_dist = euclideanDistance(0, r, g, b);
            for (int i = 1; i < m_k; i++)
            {
                distance = euclideanDistance(i, r, g, b);
                if (distance < min_dist)
                {
                    min_dist = distance;
                    centroidLabel = i;
                    m_labels(x, y) = centroidLabel;
                }
            }
        }
    }
}

void KMeans::computeCentroids()
{
    for (int i = 0; i < m_k; i++)
    {
        double mean_r = 0.0, mean_g = 0.0, mean_b = 0.0;

        int n = 0;
        for (int y = 0; y < m_h; y++)
        {
            for (int x = 0; x < m_w; x++)
            {
                if (m_labels(x, y) == i)
                {
                    mean_r += double(m_image(x, y, 0));
                    mean_g += double(m_image(x, y, 1));
                    mean_b += double(m_image(x, y, 2));

                    n++;
                }
            }
        }
        mean_r /= n;
        mean_g /= n;
        mean_b /= n;
        m_clusterCentres(i, 0, 0, 0) = mean_r;
        m_clusterCentres(i, 0, 0, 1) = mean_g;
        m_clusterCentres(i, 0, 0, 2) = mean_b;
    }
}

double KMeans::euclideanDistance(int clusterCenterIndex, int x, int y, int z) const
{
    int x0 = m_clusterCentres(clusterCenterIndex, 0, 0, 0);
    int y0 = m_clusterCentres(clusterCenterIndex, 0, 0, 1);
    int z0 = m_clusterCentres(clusterCenterIndex, 0, 0, 2);
    return sqrt(pow(x0 - x, 2) + pow(y0 - y, 2) + pow(z0 - z, 2));
}
