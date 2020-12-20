//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <memory>
#include <unordered_set>
#include <map>

struct AOC_Output {
  long value;
  auto operator()() const { return value; }
};

enum class Edge { left, right, top, bottom };
enum class Flip { x, y, xy, normal };
using match_state = std::pair<Edge, Flip>;

struct Image {
  std::vector<std::string> img{};
  long title{};

  std::string left{};
  std::string right{};
  std::string top{};
  std::string bottom{};

  void flip_x() {
    std::swap(left, right);
    std::reverse(top.begin(), top.end());
    std::reverse(bottom.begin(), bottom.end());

    for (auto &line : img)
      std::reverse(line.begin(), line.end());
  }

  void flip_y() {
    std::swap(top, bottom);
    std::reverse(left.begin(), left.end());
    std::reverse(right.begin(), right.end());

    std::reverse(img.begin(), img.end());
  }

  void rot_right(int step) {
    while (step > 0) {
      auto ntop = left, nright = top, nbot = right, nleft = bottom;
      top = ntop;
      right = nright;
      bottom = nbot;
      left = nleft;

      std::reverse(top.begin(), top.end());
      std::reverse(bottom.begin(), bottom.end());

      auto new_img = img;
      for (auto i = 0u; i < img.size(); ++i)
        for (auto j = 0u; j < img.size(); ++j){
          new_img[j][img.size() - i - 1] = img[i][j];
        }
      img = new_img;

      --step;
    }
  }

  void align_with(Edge lhs, match_state m) {
    if (m.second == Flip::x) flip_x();
    if (m.second == Flip::y) flip_y();
    if (m.second == Flip::xy) {
      flip_x();
      flip_y();
    }

    if (lhs == Edge::right) {
      switch (m.first) {
        case Edge::left:
          break;
        case Edge::top:
          rot_right(3);
          flip_y();
          break;
        case Edge::right:
          rot_right(2);
          flip_y();
          break;
        case Edge::bottom:
          rot_right(1);
          break;
      }
    }

    if (lhs == Edge::top) {
      switch (m.first) {
        case Edge::left:
          rot_right(3);
          break;
        case Edge::top:
          rot_right(2);
          flip_x();
          break;
        case Edge::right:
          rot_right(1);
          flip_x();
          break;
        case Edge::bottom:
          break;
      }
    }

    if (lhs == Edge::left) {
      switch (m.first) {
        case Edge::left:
          rot_right(2);
          flip_y();
          break;
        case Edge::top:
          rot_right(1);
          break;
        case Edge::right:
          break;
        case Edge::bottom:
          rot_right(3);
          flip_y();
          break;
      }
    }

    if (lhs == Edge::bottom) {
      switch (m.first) {
        case Edge::left:
          rot_right(1);
          flip_x();
          break;
        case Edge::top:
          break;
        case Edge::right:
          rot_right(3);
          break;
        case Edge::bottom:
          rot_right(2);
          flip_x();
          break;
      }
    }
  }
};

void assert(auto const &lhs, auto const &rhs){
  if (lhs != rhs) {
    std::cout << lhs << "!=" << rhs << std::endl;
    throw lhs + "!=" + rhs;
  }
}

std::optional<match_state> try_match(std::string const &edge, Image const &rhs) {
  if (edge == rhs.left) return {{Edge::left, Flip::normal}};
  if (edge == rhs.right) return {{Edge::right, Flip::normal}};
  if (edge == rhs.top) return {{Edge::top, Flip::normal}};
  if (edge == rhs.bottom) return {{Edge::bottom, Flip::normal}};

  auto xflip = rhs;
  xflip.flip_x();

  if (edge == xflip.left) return {{Edge::left, Flip::x}};
  if (edge == xflip.right) return {{Edge::right, Flip::x}};
  if (edge == xflip.top) return {{Edge::top, Flip::x}};
  if (edge == xflip.bottom) return {{Edge::bottom, Flip::x}};

  auto yflip = rhs;
  yflip.flip_y();

  if (edge == yflip.left) return {{Edge::left, Flip::y}};
  if (edge == yflip.right) return {{Edge::right, Flip::y}};
  if (edge == yflip.top) return {{Edge::top, Flip::y}};
  if (edge == yflip.bottom) return {{Edge::bottom, Flip::y}};


  auto xyflip = yflip;
  xyflip.flip_x();

  if (edge == xyflip.left) return {{Edge::left, Flip::xy}};
  if (edge == xyflip.right) return {{Edge::right, Flip::xy}};
  if (edge == xyflip.top) return {{Edge::top, Flip::xy}};
  if (edge == xyflip.bottom) return {{Edge::bottom, Flip::xy}};

  return {};
}

struct AOC_Input {
  Image img;

  explicit AOC_Input(Image _img) : img{std::move(_img)} {
    img.top = img.img.front();
    img.bottom = img.img.back();

    for (auto &line : img.img) {
      img.left += line.front();
      img.right += line.back();
    }
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  static Image img;
  if (line.empty()) {
    auto copy = img;
    img = Image();
    return AOC_Input(copy);
  } else {
    if (line[0] == 'T') {
      std::istringstream iss{line};
      std::string tmp;
      iss >> tmp;
      iss >> img.title;
    } else
      img.img.emplace_back(line);
  }
  return {};
}

struct ImageEntry {
  Image *frame;
  std::unique_ptr<ImageEntry> left = nullptr, top = nullptr, right = nullptr, bottom = nullptr;
};

void fill_neight(std::unique_ptr<ImageEntry> &head, std::unordered_set<long> &used_id, std::vector<AOC_Input> &imgs) {
//LEFT < --
  for (auto &img : imgs) {
    if (!used_id.contains(img.img.title)) {
      auto match = try_match(head->frame->left, img.img);
      if (match) {
        img.img.align_with(Edge::left, match.value());

        assert(head->frame->left, img.img.right);

        used_id.insert(img.img.title);
        head->left = std::make_unique<ImageEntry>();
        head->left->frame = &(img.img);
        fill_neight(head->left, used_id, imgs);

        break;
      }
    }
  }

//RIGHT -->
  for (auto &img : imgs) {
    if (!used_id.contains(img.img.title)) {
      auto match = try_match(head->frame->right, img.img);
      if (match) {
        img.img.align_with(Edge::right, match.value());

        assert(head->frame->right, img.img.left);

        used_id.insert(img.img.title);
        head->right = std::make_unique<ImageEntry>();
        head->right->frame = &(img.img);
        fill_neight(head->right, used_id, imgs);

        break;
      }
    }
  }

  //    ^
  //    |
  //TOP
  for (auto &img : imgs) {
    if (!used_id.contains(img.img.title)) {
      auto match = try_match(head->frame->top, img.img);
      if (match) {
        img.img.align_with(Edge::top, match.value());

        assert(head->frame->top, img.img.bottom);

        used_id.insert(img.img.title);
        head->top = std::make_unique<ImageEntry>();
        head->top->frame = &(img.img);
        fill_neight(head->top, used_id, imgs);

        break;
      }
    }
  }

  //  |
  //  v
  //Bottom
  for (auto &img : imgs) {
    if (!used_id.contains(img.img.title)) {
      auto match = try_match(head->frame->bottom, img.img);
      if (match) {
        img.img.align_with(Edge::bottom, match.value());

        assert(head->frame->bottom, img.img.top);

        used_id.insert(img.img.title);
        head->bottom = std::make_unique<ImageEntry>();
        head->bottom->frame = &(img.img);
        fill_neight(head->bottom, used_id, imgs);

        break;
      }
    }
  }
}

bool operator< (std::pair<long, long> const &lhs, std::pair<long, long> const &rhs){
  return (lhs.first == rhs.first) ? lhs.second < rhs.second : lhs.first < rhs.first;
}

void create_map(std::map<std::pair<long, long>, Image*> &mp, std::unique_ptr<ImageEntry> const &head, auto i, auto j){
  if (head == nullptr)
    return;

  mp[{i, j}] = head->frame;
  create_map(mp, head->left, i - 1, j);
  create_map(mp, head->right, i + 1, j);
  create_map(mp, head->top, i, j + 1);
  create_map(mp, head->bottom, i, j - 1);
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto raw_pic = v;
  auto head = std::make_unique<ImageEntry>();
  head->frame = &(raw_pic.front().img);

  std::unordered_set used{raw_pic.front().img.title};
  fill_neight(head, used, raw_pic);

  std::map<std::pair<long, long>, Image*> mp;
  create_map(mp, head, 0, 0);

  auto min_i = 0, min_j = 0;
  auto max_i = 0, max_j = 0;

  for (auto i = -100; i < 100; ++i)
    for (auto j = -100; j < 100; ++j){
      if (mp.contains({j, i})){
        if (min_i > i) min_i = i;
        if (max_i < i) max_i = i;

        if (min_j > j) min_j = j;
        if (max_j < j) max_j = j;
        //std::cout << mp[{j, i}] << " ";
      }
    }

  auto result = mp[{min_j, min_i}]->title * mp[{max_j, min_i}]->title * mp[{min_j, max_i}]->title * mp[{max_j, max_i}]->title;

  return {result};
}

bool monster_matching(std::vector<std::string> &pic){
  std::array<std::string, 3> monster = {"                  # ",
                                        "#    ##    ##    ###",
                                        " #  #  #  #  #  #   "};
  auto match_monster = [&monster, &pic](auto sx, auto sy){
    auto count =  15;
    for (auto i = 0; i < 3; ++i)
      for (auto j = 0u; j < monster.front().size(); ++j){
        if (monster[i][j] == '#' && pic[sx + i][sy + j] == monster[i][j])
          --count;
      }
    return count == 0;
  };

  auto fill_monster = [&monster, &pic](auto sx, auto sy){
    for (auto i = 0; i < 3; ++i)
      for (auto j = 0u; j < monster.front().size(); ++j){
        if (monster[i][j] == '#' && pic[sx + i][sy + j] == monster[i][j])
          pic[sx + i][sy + j] = 'O';
      }
  };

  auto find_monster = false;
  for (auto i = 0u; i < pic.size() - monster.size(); ++i)
    for (auto j = 0u; j < pic.front().size() - monster.front().size(); ++j){
      if (match_monster(i, j)){
        find_monster = true;
        fill_monster(i, j);
      }
    }

  return find_monster;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto raw_pic = v;
  auto head = std::make_unique<ImageEntry>();
  head->frame = &(raw_pic.front().img);

  std::unordered_set used{raw_pic.front().img.title};
  fill_neight(head, used, raw_pic);

  std::map<std::pair<long, long>, Image*> mp;
  create_map(mp, head, 0, 0);

  auto min_i = 0, min_j = 0;
  auto max_i = 0, max_j = 0;

  for (auto i = -100; i < 100; ++i)
    for (auto j = -100; j < 100; ++j){
      if (mp.contains({j, i})){
        if (min_i > i) min_i = i;
        if (max_i < i) max_i = i;

        if (min_j > j) min_j = j;
        if (max_j < j) max_j = j;
      }
    }

  auto height = max_i - min_i + 1;

  std::vector<std::string> pic(height * 8, "");
  for (auto i = max_i, x = 0; i >= min_i; --i, ++x)
    for (auto j = min_j; j <= max_j; ++j){
      auto shift = x * 8;
      for (auto k = 1; k < 9; ++k){
        pic[shift + k - 1].append(mp[{j, i}]->img[k].begin() + 1, mp[{j, i}]->img[k].end() - 1);
      }
    }

  Image pimg;
  pimg.img = pic;
  pimg.flip_y();

  auto rotation_search = [](auto &pic){
    for (auto i = 1; i <= 3; ++i){
      pic.rot_right(i);
      auto mm = monster_matching(pic.img);
      if (mm)
        return true;
    }
    return false;
  };

  auto count = [](auto &img){
    return std::accumulate(img.img.begin(), img.img.end(), 0ll, [](auto sum, auto const &line){
      return sum + std::count(line.begin(), line.end(), '#');
    });
  };

  if (rotation_search(pimg)){
    return {count(pimg)};
  }

  auto xpimg = pimg; xpimg.flip_x();
  if (rotation_search(xpimg)){
    return {count(xpimg)};
  }

  auto ypimg = pimg; ypimg.flip_y();
  if (rotation_search(ypimg)){
    return {count(ypimg)};
  }

  auto xypimg = pimg; xypimg.flip_x(); xypimg.flip_y();
  if (rotation_search(xypimg)){
    return {count(xypimg)};
  }

  return {2};
}

#include "exec.hpp"