/**
    main.cpp
    Purpose: Find visual correspondences between two sets of
    features from a pair of images

    @author Leiver Andres Campeón <leiverandres04p@hotmail.com>
    @author Yensy Helena Gomez <yensy@sirius.utp.edu.co>
    @author Juan Sebastián Vega Patiño <sebas060495@gmail.com>
    @version 1.0 29/12/16
*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <getopt.h>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "match.hpp"
#include "draw.hpp"

using namespace cv;
using namespace std;


/*
##     ## ######## #### ##        ######
##     ##    ##     ##  ##       ##    ##
##     ##    ##     ##  ##       ##
##     ##    ##     ##  ##        ######
##     ##    ##     ##  ##             ##
##     ##    ##     ##  ##       ##    ##
 #######     ##    #### ########  ######
*/

/**
  Sums up elements of a vector

  @param vec vector containing elements to accumulate
  @return sum of elements
*/
long double accum(vector<double> &vec) {
  long double sum = 0.0;
  vector<double>::iterator it;
  for (it = vec.begin(); it != vec.end(); it++) {
    sum += *it;
  }
  return sum;
}

template<typename T>
vector<vector<T> > getPermutation(vector<T> data) {
  vector<vector<T> > perms;
  sort(data.begin(), data.end());
  do {
    perms.push_back(data);
  } while(next_permutation(data.begin(), data.end()));
  return perms;
}

bool responseCMP(const KeyPoint& p1, const KeyPoint& p2) {
    return p1.response > p2.response;
}

/*
######## ########   ######   ########  ######
##       ##     ## ##    ##  ##       ##    ##
##       ##     ## ##        ##       ##
######   ##     ## ##   #### ######    ######
##       ##     ## ##    ##  ##             ##
##       ##     ## ##    ##  ##       ##    ##
######## ########   ######   ########  ######
*/

/**
  Obtain a list of hyperedges from the Delaunay Triangulation computed with
  some Image Keypoints

  @param img Image from which keypoints are extracted
  @param kpts
  @return
*/

vector<vector<int> > delaunayTriangulation(Mat img, vector<KeyPoint> kpts) {
  vector<Point2f> points;
  KeyPoint::convert(kpts, points);
  map<pair<double, double> , int> pt_idx;

  // Mapping points with their indices
  for (int i = 0; i < points.size(); i++) {
    Point2f p = points[i];
    pt_idx[make_pair(p.x, p.y)] = i;
  }

  // Triangulation
  Size size = img.size();
  Rect rect(0, 0, size.width, size.height);
  Subdiv2D subdiv(rect);
  subdiv.insert(points);
  vector<Vec6f> triangleList;
  subdiv.getTriangleList(triangleList);

  draw::triangulation(img, triangleList);

  // Converting to edges from coordinates to indices
  int rect_count_outliers = 0;
  int map_count_outliers = 0;
  vector<Point2f> pt(3);
  vector<vector<int> > edges;
  for (int i = 0; i < triangleList.size(); i++) {
    Vec6f t = triangleList[i];
    pt[0] = Point2f(t[0], t[1]);
    pt[1] = Point2f(t[2], t[3]);
    pt[2] = Point2f(t[4], t[5]);
    if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
      pair<double, double> p0 = make_pair(pt[0].x, pt[0].y);
      pair<double, double> p1 = make_pair(pt[1].x, pt[1].y);
      pair<double, double> p2 = make_pair(pt[2].x, pt[2].y);
      if (pt_idx.count(p0) && pt_idx.count(p1) && pt_idx.count(p2)) {
        vector<int> edge(3);
        edge[0] = pt_idx[p0];
        edge[1] = pt_idx[p1];
        edge[2] = pt_idx[p2];
        edges.push_back(edge);
      } else {
        map_count_outliers++;
      }
    } else {
      rect_count_outliers++;
    }
  }

  return edges;
}

/*
##     ##    ###    #### ##    ##
###   ###   ## ##    ##  ###   ##
#### ####  ##   ##   ##  ####  ##
## ### ## ##     ##  ##  ## ## ##
##     ## #########  ##  ##  ####
##     ## ##     ##  ##  ##   ###
##     ## ##     ## #### ##    ##
*/

void do_match(Mat &img1, Mat &img2, double cang, double crat, double cdesc) {
  // Mat img1 = imread("./test-images/monster1s.JPG", 0);
  // Mat img2 = imread("./test-images/monster1m.JPG", 0);

  // For Surf detection
  int minHessian = 400;

  SurfFeatureDetector detector(minHessian);
  // SiftFeatureDetector detector(0.05, 5.0);
  vector<KeyPoint> kpts1, kpts2;
  detector.detect(img1, kpts1);
  detector.detect(img2, kpts2);

  cout << endl << kpts1.size() << " Keypoints Detected in image 1" << endl;
  cout << endl << kpts2.size() << " Keypoints Detected in image 2" << endl;

  sort(kpts1.begin(), kpts1.end(), responseCMP);
  sort(kpts2.begin(), kpts2.end(), responseCMP);

  // Test vectors with less points
  int limit = 20;
  vector<KeyPoint> t_kpts1(kpts1.begin(), kpts1.begin() + limit);
  vector<KeyPoint> t_kpts2(kpts2.begin(), kpts2.begin() + limit);

  SurfDescriptorExtractor extractor;
  // SiftDescriptorExtractor extractor;
  Mat descriptor1, descriptor2;
  extractor.compute(img1, kpts1, descriptor1);
  extractor.compute(img2, kpts2, descriptor2);

  // Add results to an image and save them.
  Mat output1;
  Mat output2;

  // Save images with keypoints
  // drawKeypoints(img1, kpts1, output1);
  // imwrite("surf_result1.jpg", output1);
  // drawKeypoints(img2, kpts2, output2);
  // imwrite("surf_result2.jpg", output2);

  // Building hyperedges Matrices
  cout << endl << "Triangulating ..." << endl;
  vector<vector<int> > Edges1 = delaunayTriangulation(img1, kpts1);
  vector<vector<int> > Edges2 = delaunayTriangulation(img2, kpts2);

  cout << endl << "Triangulation Done." << endl;
  cout << Edges1.size() << " Edges from image 1" << endl;
  cout << Edges2.size() << " Edges from image 2" << endl;
  cout << endl << "Matching ..." << endl;

  vector<pair<int, int> > edge_matches = match::hyperedges(
    Edges1, Edges2,
    kpts1, kpts2,
    descriptor1, descriptor2,
    cang, crat, cdesc, 0.40
  );

  cout << endl << "Edges Matching done. ";
  cout << edge_matches.size() << " edge matches passed!" << endl;

  vector<DMatch> matches = match::points(
    edge_matches, descriptor1, descriptor2,  Edges1, Edges2, 0.1
  );

  cout << endl << "Point Matching Done. ";
  cout << matches.size() << " Point matches passed!" << endl;

  // Draw Edges matching
  // draw::edgesMatch(
  //   img1, img2, edge_matches, Edges1, Edges2, kpts1, kpts2
  // );

  // Draw Point matching
  draw::pointsMatch(img1, kpts1, img2, kpts2, matches);
}

void usage(char* program_name) {
    // opts = ['--cang', '--crat', '--cdesc']
    // description = [
    //     'Constant of angle similarity (default: 1)',
    //     'Constant of ratio similarity (default: 1)',
    //     'Constant of SURF descriptor similarity (default: 1)'
    // ]
    //
    // print 'Usage: {} [options ...] img1 img2'.format(sys.argv[0])
    // print
    // print 'Matching options:'
    // for o, d in zip(opts, description):
    //     print '  {}: {}'.format(o, d)
    // sys.exit(2)
  int n = 3;
  string opts[] = {"--cang", "--crat", "--cdesc"};
  string description[] = {
    "Constant of angle similarity (default: 1)",
    "Constant of ratio similarity (default: 1)",
    "Constant of SURF descriptor similarity (default: 1)"
  };

  cout << "Usage: " << program_name << " [options ...] img1 img2" << endl;
  cout << endl;
  cout << "Matching options" << endl;
  for (int i = 0; i < n; i++) {
    cout << "  " << opts[i] << ": " << description[i] << endl;
  }

  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int opt, opt_index = 0;
  static struct option options[] = {
    {"cang", required_argument, 0, 'a'},
    {"crat", required_argument, 0, 'r'},
    {"cdesc", required_argument, 0, 'd'},
    {0, 0, 0, 0}
  };

  double cang = 1, crat = 1, cdesc = 1;
  while ((opt = getopt_long(argc, argv, "a:r:d:", options, &opt_index)) != -1) {
    switch (opt) {
      case 'a':
        cang = atof(optarg);
        break;
      case 'r':
        crat = atof(optarg);
        break;
      case 'd':
        cdesc = atof(optarg);
        break;
      default:
        usage(argv[0]);
        break;
    }
  }

  cout << cang << " " << crat << " " << cdesc << endl;

  return 0;
}
