#include "iostream"
#include "stdio.h"
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

// Distancia ecluidinana entre los puntos de una misma imagen usando
// descriptores
// Mat euclidDistance(Mat &vect1) {
//   Mat matrixEuclidian(vect1.rows, vect1.rows, DataType<double>::type);
//   for (int i = 0; i < vect1.rows; i++){
//     for (int j = 0; j < vect1.rows; j++){
//       double sum = 0.0;
//       for (int k = 0; k < vect1.cols; k++){
//         double e_i = vect1.at<double>(k, i);
//         double e_j = vect1.at<double>(k, j);
//         cout << e_i << " " << e_j << " : " << ((e_i - e_j) * (e_i - e_j)) << endl;
//         sum += ((e_i - e_j) * (e_i - e_j));
//       }
//       matrixEuclidian.at<double>(i,j) = sqrt(sum);
//     }
//   }
//   cout << "Distancias en la función" << endl;
//   for (int i = 0; i < vect1.rows; i++) {
//     for (int j = 0; j < vect1.rows; j++) {
//       cout << matrixEuclidian.at<double>(i, j) << " ";
//     }
//     cout << endl;
//   }
//   return matrixEuclidian;
// }

// //Distancias entre los puntos caracteristicas de una imagen1 usando la
// posición del punto.
// Mat distancePoints(vector<KeyPoint> &point) {
//   int n = point.size();
//   Mat matrixEuclidian(n, n, DataType<float>::type),
//       matrixEuclidianSort(n, n, DataType<int>::type);
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       float x1 = point[i].pt.x;
//       float x2 = point[j].pt.x;
//       float y1 = point[i].pt.y;
//       float y2 = point[j].pt.y;
//       matrixEuclidian.at<float>(i, j) =
//           sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
//     }
//   }
//   cv::sortIdx(matrixEuclidian, matrixEuclidianSort , CV_SORT_ASCENDING);
//
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       cout << matrixEuclidian.at<float>(i, j) << " ";
//     }
//     cout << endl;
//   }
//   cout<<"ordenado" <<endl;
//   for (int i = 0; i < 10; i++) {
//     for (int j = 0; j < 10; j++) {
//       cout << matrixEuclidianSort.at<int>(i,j) << " ";
//     }
//     cout << endl;
//   }
//   return matrixEuclidian;
// }

/*
  sen(theta) = V1 dot V2 / |V1| * |V2|, ésta es una medida de similaridad entre
  descriptores de dos puntos, entre mayor sea el valor de similarity, más parecidos
  son los descriptores. El calculo se hace para cada pareja entre los puntos de las
  dos imagenes.
*/
// Mat distanceBetweenImg(Mat &vec1, Mat &vec2) {
//   Mat similarity(vec1.rows, vec2.rows, DataType<float>::type);
//   for (int i = 0; i < vec1.rows; i++) {
//     for (int j = 0; j < vec2.rows; j++) {
//       float producto = 0.0;
//       float norma1 = 0.0;
//       float norma2 = 0.0;
//       for (int k = 0; k < vec1.cols; k++) {
//         norma1 += vec1.at<float>(i, k)*vec1.at<float>(i, k);
//         norma2 += vec2.at<float>(j, k)*vec2.at<float>(j, k);
//         producto += (vec1.at<float>(i, k) * vec2.at<float>(j, k));
//       }
//
//       similarity.at<float>(i, j) = (producto / ((sqrt(norma1) * sqrt(norma2))));
//     }
//   }
//   for (int i = 0; i < 10; i++) {
//     for (int j = 0; j < 10; j++) {
//       if (similarity.at<double>(i,j)> 0.5000000) {
//         cout << similarity.at<float>(j, i) << " ";
//       }
//     }
//     cout << endl;
//   }
//   return similarity;
// }

/*
* Algoritmo de los k vecinos más cercanos, esto nos permitira conocer
* los indices de la matrix de los vecinos más cercanos,
* para hacer el hipergrafo de la img1 como de img2
*/
// Mat KNN(Mat &matEucl) {
//   Mat indices(matEucl.rows, 3, DataType<int>::type);
//   float minDist = 10e6;
//   int minIdx1 = -1;
//   int minIdx2 = -1;
//   for (int i = 0; i < matEucl.rows; i++) {
//     for (int j = 0; j < matEucl.cols; j++) {
//       if ((matEucl.at<float>(i, j) <= minDist) && (j != i)) {
//         minDist = matEucl.at<float>(i, j);
//         minIdx1 = j;
//       }
//     }
//     minDist = 1e6;
//     for (int j = 0; j < matEucl.cols; j++) {
//       if ((matEucl.at<float>(i, j) <= minDist) && (j != minIdx1) && (j != i)) {
//         minDist = matEucl.at<float>(i, j);
//         minIdx2 = j;
//       }
//     }
//     cout << "Indice0: " << i << " "
//          << "indice1: " << minIdx1 << " "
//          << "indice2: " << minIdx2 << endl;
//     indices.at<int>(i, 0) = i;
//     indices.at<int>(i, 1) = minIdx1;
//     indices.at<int>(i, 2) = minIdx2;
//   }
//   return indices;
// }

// float positionXYIJK(Mat &indice, vector<KeyPoint> &point){
//   float size = indice.rows*sizeof(float);
//   float  *determinant;
//   determinant = (float *) malloc(size);
//
//   for (int i = 0; i < indice.rows; i++) {
//       float x1 = point[indice.at<int>(i, 0)].pt.x;
//       float y1 = point[indice.at<int>(i, 0)].pt.y;
//       float x2 = point[indice.at<int>(i, 1)].pt.x;
//       float y2 = point[indice.at<int>(i, 1)].pt.y;
//       float x3 = point[indice.at<int>(i, 2)].pt.x;
//       float y3 = point[indice.at<int>(i, 2)].pt.y;
//       determinant[i] = (x1-x3)*(y2-y3)-(x2-x3)*(y1-y3);
//       cout << "V1: " << x1 << ", " << y1 << endl;
//       cout << "V2: " << x2 << ", " << y2 << endl;
//       cout << "V3: " << x3 << ", " << y3 << endl;
//       cout << determinant[i] << endl;
//
//     }
//   return *determinant;
//   free(determinant);
// }

/*
########  ########     ###    ##      ## #### ##    ##  ######
##     ## ##     ##   ## ##   ##  ##  ##  ##  ###   ## ##    ##
##     ## ##     ##  ##   ##  ##  ##  ##  ##  ####  ## ##
##     ## ########  ##     ## ##  ##  ##  ##  ## ## ## ##   ####
##     ## ##   ##   ######### ##  ##  ##  ##  ##  #### ##    ##
##     ## ##    ##  ##     ## ##  ##  ##  ##  ##   ### ##    ##
########  ##     ## ##     ##  ###  ###  #### ##    ##  ######
*/

void drawDelaunay(Mat &img, vector<Vec6f> &triangleList) {
  Mat img_out;
  img.copyTo(img_out);
  Scalar delaunay_color(255,255,255);
  vector<Point> pt(3);
  Size size = img_out.size();
  Rect rect(0, 0, size.width, size.height);
  int count_outliers = 0;
  for( size_t i = 0; i < triangleList.size(); i++ ) {
    Vec6f t = triangleList[i];
    pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
    pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
    pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

    // Draw rectangles completely inside the image.
    if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
      line(img_out, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
      line(img_out, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
      line(img_out, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
    } else {
      count_outliers++;
    }
  }
  // cout << "[drawDelaunay] " <<count_outliers << " points are not in rect" << endl;
  imshow("Delaunay Triangulation", img_out);
  waitKey(0);
}

void drawEdgesMatch(Mat &img1, Mat &img2, vector< pair<int, int> > &matches,
                    vector<vector<int> > &edge1, vector<vector<int> > &edge2,
                    vector<KeyPoint> &kpts1, vector<KeyPoint> &kpts2) {
  Mat img_aux, img_out;

  for (int i = 0; i < matches.size(); i++) {
    hconcat(img1, img2, img_aux);
    cvtColor(img_aux, img_out, CV_GRAY2RGB);
    int base_idx = matches[i].first;
    int ref_idx  = matches[i].second;
    Point2f p0 = kpts1[edge1[base_idx][0]].pt;
    Point2f p1 = kpts1[edge1[base_idx][1]].pt;
    Point2f p2 = kpts1[edge1[base_idx][2]].pt;

    Point2f q0 = kpts2[edge2[ref_idx][0]].pt;
    Point2f q1 = kpts2[edge2[ref_idx][1]].pt;
    Point2f q2 = kpts2[edge2[ref_idx][2]].pt;

    circle(img_out, p0, 2, Scalar(0, 0, 255), 3);
    circle(img_out, p1, 2, Scalar(0, 0, 255), 3);
    circle(img_out, p2, 2, Scalar(0, 0, 255), 3);
    circle(img_out, q0 + Point2f(img1.cols, 0), 2, Scalar(0, 255, 0), 3);
    circle(img_out, q1 + Point2f(img1.cols, 0), 2, Scalar(0, 255, 0), 3);
    circle(img_out, q2 + Point2f(img1.cols, 0), 2, Scalar(0, 255, 0), 3);
    imshow("Hyperedge Matching", img_out);
    waitKey(0);
  }
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

vector<vector<int>> delaunayTriangulation(Mat img, vector<KeyPoint> kpts) {
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
  Rect rect(0, 0, size.width, size.height); // TODO
  Subdiv2D subdiv(rect);
  subdiv.insert(points);
  vector<Vec6f> triangleList;
  subdiv.getTriangleList(triangleList);

  drawDelaunay(img, triangleList);

  // Converting to edges with coordinates to indices
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

  // DEBUG
  // cout << "[delaunayTriangulation] ";
  // cout << "Size of input KeyPoints " << kpts.size() << endl;
  // cout << "[delaunayTriangulation] ";
  // cout << "Size of input points " << points.size() << endl;
  // cout << "[delaunayTriangulation] ";
  // cout << "Size of trinagleList: " << triangleList.size() << endl;
  // cout << "[delaunayTriangulation] ";
  // cout << "Number of Rect outliers " << rect_count_outliers << endl;
  // cout << "[delaunayTriangulation] ";
  // cout << "Number of Map outliers " << map_count_outliers << endl;
  // cout << "[delaunayTriangulation] ";
  // int outliers = rect_count_outliers + map_count_outliers;
  // if ((triangleList.size() - outliers) != edges.size()) {
  //   cout << "Edges has more elements than it should" << endl;
  // } {
  //   cout << "Edges has exactly (trinagleList.size() - outliers) elements: " <<
  //         edges.size() << endl;
  // }
  // for (int i = 0; i < edges.size(); i++) {
  //   cout << "Edges number " << i << " : " << endl;
  //   cout << edges[i][0] << " ";
  //   cout << edges[i][1] << " ";
  //   cout << edges[i][2] << endl;
  // }

  return edges;
}

/*
 ######  #### ##     ## #### ##          ###    ########  #### ######## ##    ##
##    ##  ##  ###   ###  ##  ##         ## ##   ##     ##  ##     ##     ##  ##
##        ##  #### ####  ##  ##        ##   ##  ##     ##  ##     ##      ####
 ######   ##  ## ### ##  ##  ##       ##     ## ########   ##     ##       ##
      ##  ##  ##     ##  ##  ##       ######### ##   ##    ##     ##       ##
##    ##  ##  ##     ##  ##  ##       ##     ## ##    ##   ##     ##       ##
 ######  #### ##     ## #### ######## ##     ## ##     ## ####    ##       ##
*/

vector<vector<int> > getCombination(int n, int r) {
  vector<bool> v(n);
  vector<vector<int> > combinations;
  fill(v.begin(), v.begin() + r, true);
  do {
    vector<int> one_combination;
    for (int i = 0; i < n; ++i) {
      if (v[i]) {
          one_combination.push_back(i);
      }
    }
    combinations.push_back(one_combination);
  } while (prev_permutation(v.begin(), v.end()));
  return combinations;
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

/*
 Calculating area of triangles with Heron's formula
 Area = sqrt(s*(s - a)*(s - b)*(s - c))
 Where:
 a, b, c are triangle sides
 s = (a + b + c) / 2
*/
double areaSimilarity(vector<int> e1, vector<int> e2, vector<KeyPoint> &kpts1,
                      vector<KeyPoint> &kpts2, double sigma) {
  vector<Point2f> p(3), q(3);
  for (int i = 0; i < 3; i++) {
    p[i] = kpts1[e1[i]].pt;
    q[i] = kpts2[e2[i]].pt;
  }

  vector<double> p_sides, q_sides;
  vector<vector<int> > comb_indices = getCombination(3, 2);
  for (int i = 0; i < comb_indices.size(); i++) {
    long double p_side = norm(p[comb_indices[i][0]] - p[comb_indices[i][1]]);
    long double q_side = norm(q[comb_indices[i][0]] - q[comb_indices[i][1]]);
    p_sides.push_back(p_side);
    q_sides.push_back(q_side);
  }

  long double s_p = accumulate(p_sides.begin(), p_sides.end(), 0.0) / 2.0;
  long double s_q = accumulate(q_sides.begin(), q_sides.end(), 0.0) / 2.0;
  long double p_area = sqrt(s_p*(s_p - p_sides[0])*(s_p - p_sides[1])*(s_p - p_sides[2]));
  long double q_area = sqrt(s_q*(s_q - q_sides[0])*(s_q - q_sides[1])*(s_q - q_sides[2]));
  long double similarity = exp(- abs(sqrt(p_area) - sqrt(q_area)) / sigma);

  // DEBUG
  // cout << "[areaSimilarity] ";
  // cout << "P points " << p << endl;
  // cout << "[areaSimilarity] ";
  // cout << "Q points " << q << endl;
  // cout << "[areaSimilarity] ";
  // cout << "P sides: ";
  // for (int i = 0; i < 3; i++) {
  //   cout << p_sides[i] << " ";
  // }
  // cout << endl;
  // cout << "[areaSimilarity] ";
  // cout << "Q sides: ";
  // for (int i = 0; i < 3; i++) {
  //   cout << q_sides[i] << " ";
  // }
  // cout << endl;
  // cout << "[areaSimilarity] ";
  // cout << "P sides s param " << s_p << endl;
  // cout << "[areaSimilarity] ";
  // cout << "Q sides s param " << s_q << endl;
  // cout << "[areaSimilarity] ";
  // cout << "P-area: " << sqrt(p_area) << " Q-area: " << sqrt(q_area) << endl;
  // cout << "[areaSimilarity] ";
  // cout << "resta areas " << abs(sqrt(p_area) - sqrt(q_area)) << endl;
  // cout << "[areaSimilarity] ";
  // cout << " Similarity = " << similarity << endl;

  return similarity;
}

double angleSin(Point2f &pivot, Point2f &p, Point2f &q) {
  Point2f v1 = p - pivot;
  Point2f v2 = q - pivot;
  double dot = v1.dot(v2);
  double angle = acos(dot / (norm(v1) * norm(v2)));
  double sine = sin(angle);

  // DEBUG
  // cout << "[angleSin] ";
  // cout << "Points pivot: " << pivot << " p: " << p << " q: " << q << endl;
  // cout << "[angleSin] ";
  // cout << "Vectors formed V1: " << v1 << " V2: " << v2 << endl;
  // cout << "[angleSin] ";
  // cout << "Dot " << dot << endl;
  // cout << "[angleSin] ";
  // cout << "Normas norm v1: " << norm(v1) << " norm v2: " << norm(v2) << endl;
  // cout << "[angleSin] ";
  // cout << "Cos angle: " << (dot / (norm(v1) * norm(v2))) << endl;
  // cout << "[angleSin] ";
  // cout << "Angle: " << angle << " sin angle: " << sine << endl << endl;

  return sine;
}

vector<double> getAnglesSines(vector<int> &edge, vector<KeyPoint> &kpts) {
  vector<double> angle_sines(3, 0.0);
  Point2f p1 = kpts[edge[0]].pt;
  Point2f p2 = kpts[edge[1]].pt;
  Point2f p3 = kpts[edge[2]].pt;
  // sin of angle of vectors p2 - p1, p3 - p1
  angle_sines[0] = angleSin(p1, p2, p3);
  // sin of angle of vectors p1 - p2, p3 - p2
  angle_sines[1] = angleSin(p2, p1, p3);
  // sin of angle of vectors p1 - p3, p2 - p3
  angle_sines[2] = angleSin(p3, p1, p2);

  // DEBUG
  // cout << "[getAnglesSines] angles: ";
  // for (int i = 0; i < 3; i++) {
  //   cout << angle_sines[i] << " ";
  // }
  // cout << endl;

  return angle_sines;
}

double anglesSimilarity(vector<int> e1, vector<int> e2, vector<KeyPoint> &kpts1,
                      vector<KeyPoint> &kpts2, double sigma) {
  vector<double> sines1 = getAnglesSines(e1, kpts1);
  vector<double> sines2 = getAnglesSines(e2, kpts2);
  vector<vector<double> > perm_sines = getPermutation(sines1);
  vector<double> diffs(perm_sines.size());
  Mat m_sines1(sines1, true);
  for (int i = 0; i < perm_sines.size(); i++) {
    Mat m_sines2(perm_sines[i], true);
    Mat_<double> m_diffs;
    absdiff(m_sines1, m_sines2, m_diffs);
    diffs[i] = m_diffs(0) + m_diffs(1) + m_diffs(2);

    // DEBUG
    // cout << "[anglesSimilarity] ";
    // cout << "sines 1: " << m_sines1 << endl;
    // cout << "[anglesSimilarity] ";
    // cout << "sines 2: " << m_sines2 << endl;
    // cout << "[anglesSimilarity] ";
    // cout << "resta " << m_diffs << endl;
    // cout << "[anglesSimilarity] ";
    // cout << "Sum " << diffs[i] << endl;
  }
  vector<double>::iterator min_diff = min_element(diffs.begin(), diffs.end());
  double similarity = exp(- (*min_diff) / sigma);

  // DEBUG
  // cout << "[anglesSimilarity] ";
  // cout << " min value " << *min_diff << endl;
  // cout << "[anglesSimilarity] ";
  // cout << "similarity " << similarity << endl << endl;

  return similarity;
}

double descSimilarity(vector<int> e1, vector<int> e2, Mat desc1, Mat desc2,
                      double sigma) {
  vector<vector<int> > perm_edge = getPermutation(e2);
  vector<double> diffs(perm_edge.size());
  for (int i = 0; i < perm_edge.size(); i++) {
    double suma = 0.0;
    for (int j = 0; j < e2.size(); j++) {
      Mat_<double> m_diffs;
      absdiff(desc1.row(j), desc2.row(perm_edge[i][j]), m_diffs);
      Mat comp;
      compare(desc1.row(j), desc2.row(perm_edge[i][j]), comp, CMP_NE);
      if (countNonZero(comp)) {
        cout << "EQUALS ";
        cout << "With norm: " << norm(m_diffs) << endl; // TODO bad diffs
      }
      suma += norm(m_diffs);
    }
    diffs[i] = suma;
  }
  vector<double>::iterator min_diff = min_element(diffs.begin(), diffs.end());
  double similarity = exp(- (*min_diff));

  cout << "[descSimilarity] ";
  cout << "Min diff: " << *min_diff << endl;
  cout << "[descSimilarity] ";
  cout << "similarity " << similarity << endl;

  return similarity;
}


/*
##     ##    ###    ########  ######  ##     ## #### ##    ##  ######
###   ###   ## ##      ##    ##    ## ##     ##  ##  ###   ## ##    ##
#### ####  ##   ##     ##    ##       ##     ##  ##  ####  ## ##
## ### ## ##     ##    ##    ##       #########  ##  ## ## ## ##   ####
##     ## #########    ##    ##       ##     ##  ##  ##  #### ##    ##
##     ## ##     ##    ##    ##    ## ##     ##  ##  ##   ### ##    ##
##     ## ##     ##    ##     ######  ##     ## #### ##    ##  ######
*/

vector< pair<int, int> > matchHyperedges(vector<vector<int> > &edges1,
                                         vector<vector<int> > &edges2,
                                         vector<KeyPoint> &kp1,
                                         vector<KeyPoint> &kp2,
                                         Mat &desc1, Mat &desc2,
                                         double c1, double c2, double c3,
                                         double thresholding) {
  double sigma = 0.5;
  vector< pair<int, int> > matches;
  double suma = c1 + c2 + c3;
  c1 /= suma;
  c2 /= suma;
  c3 /= suma;

  for (int i = 0; i < edges1.size(); i++) {
    int best_match_idx = INT_MIN;
    double max_similarity = -DBL_MIN;
    double s_ang = -DBL_MIN;
    double s_area = -DBL_MIN;
    double s_desc = -DBL_MIN;
    for (int j = 0; j < edges2.size(); j++) {
      double area_dist = areaSimilarity(edges1[i], edges2[j], kp1, kp2, sigma);
      double sim_angles = anglesSimilarity(edges1[i], edges2[j], kp1, kp2,
                                           sigma);
      double sim_desc = descSimilarity(edges1[i], edges2[j], desc1,
                                       desc2, sigma);
      double similarity = c1 * area_dist + c2 * sim_angles + c3 * sim_desc;

      cout << "[matchHyperedges] ";
      cout << "Combined Similarity " << similarity << endl;

      if (similarity > max_similarity) {
        best_match_idx = j;
        max_similarity = similarity;
        s_area = area_dist;
        s_ang = sim_angles;
        s_desc = sim_desc;
      }
    }
    if (max_similarity >= thresholding) {
      cout << "HEY one match" << endl;
      matches.push_back(make_pair(i, best_match_idx));
    }
  }
  return matches;
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

int main(int argc, const char *argv[]) {
  Mat img1 = imread("./house/house.seq0.png", 0); // Load as grayscale
  Mat img2 = imread("./house/house.seq0.png", 0);

  // For Surf detection
  int minHessian = 400;
  SurfFeatureDetector detector(minHessian);
  vector<KeyPoint> keypoints1, keypoints2;
  detector.detect(img1, keypoints1);
  detector.detect(img2, keypoints2);
  // Test vectors with less points
  int limit = 5;
  vector<KeyPoint> t_keypoints1(keypoints1.begin(), keypoints1.begin() + limit);
  vector<KeyPoint> t_keypoints2(keypoints2.begin(), keypoints2.begin() + limit);

  SurfDescriptorExtractor extractor;
  Mat descriptor1, descriptor2;
  extractor.compute(img1, t_keypoints1, descriptor1);
  extractor.compute(img2, t_keypoints2, descriptor2);

  // Add results to an image and save them.
  Mat output1;
  Mat output2;

  // suft GPU-parallel with opencv
  drawKeypoints(img1, t_keypoints1, output1);
  imwrite("sift_result1.jpg", output1);
  drawKeypoints(img2, t_keypoints2, output2);
  imwrite("sift_result2.jpg", output2);

  // Distance between every point to every point of same image
  // Mat dist1 = distancePoints(t_keypoints1);
  // Mat dist2 = distancePoints(t_keypoints2);

  // Building hyperedges Matrices
  vector<vector<int> > Edges1 = delaunayTriangulation(img1, t_keypoints1);
  vector<vector<int> > Edges2 = delaunayTriangulation(img2, t_keypoints2);

  // TODO Hyperedge matching
  vector<pair<int, int> > matches = matchHyperedges(Edges1, Edges2, t_keypoints1,
                                                    t_keypoints2, descriptor1,
                                                    descriptor2, 15, 10, 5,
                                                    0.85);
  // TODO Point matching

  // TODO Draw matching
  drawEdgesMatch(img1, img2, matches, Edges1, Edges2, t_keypoints1, t_keypoints2);
  return 0;
}
