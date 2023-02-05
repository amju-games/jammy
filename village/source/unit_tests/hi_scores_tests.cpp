#include "catch.hpp"
#include "directory.h"
#include "hi_scores.h"

TEST_CASE("populate hi scores", "[hi_scores]")
{
  hi_scores hi;
  REQUIRE(hi.get_num_hi_scores() == 0);
  hi.store(3, "ccc");
  REQUIRE(hi.get_num_hi_scores() == 1);
  hi.store(1, "aaa");
  REQUIRE(hi.get_num_hi_scores() == 2);
  hi.store(2, "bbb");
  REQUIRE(hi.get_num_hi_scores() == 3);
}

void test_save_load_hi_scores(const std::string& path)
{
  REQUIRE(make_dir(path));

  hi_scores hi;
  hi.store(3, "ccc");
  hi.store(1, "aaa");
  hi.store(2, "bbb");
  REQUIRE(hi.save(path + "hi_score_test"));

  hi_scores hi2;
  REQUIRE(hi2.load(path + "hi_score_test"));
  REQUIRE(hi2.get_num_hi_scores() == 3);
  int score = -1;
  std::string name;
  hi2.get_hi_score(0, score, name);

  // Stored in ascending order of score
  REQUIRE(score == 1);
  REQUIRE(name == "aaa");
}

TEST_CASE("save then load hi scores 1", "[hi_scores]")
{
  test_save_load_hi_scores("");
}

TEST_CASE("save then load hi scores 2", "[hi_scores]")
{
  test_save_load_hi_scores("path/to/hi/scores/");
}

TEST_CASE("is score a hi score", "[hi_scores]")
{
  hi_scores hi;
  hi.set_max_num_hi_scores(3);
  hi.store(13, "ccc");
  hi.store(11, "aaa");
  hi.store(12, "bbb");

  REQUIRE_FALSE(hi.is_hi_score(10));
  REQUIRE(hi.is_hi_score(11));
}

TEST_CASE("scores are in ascending order", "[hi_scores]")
{
  hi_scores hi;
  hi.set_max_num_hi_scores(3);
  hi.store(13, "ccc");
  hi.store(11, "aaa");
  hi.store(12, "bbb");
  int score = -1;
  std::string name;
  hi.get_hi_score(0, score, name);
  REQUIRE(score == 11);
  hi.get_hi_score(2, score, name);
  REQUIRE(score == 13);
}

TEST_CASE("don't store non-hi score", "[hi_scores]")
{
  hi_scores hi;
  hi.set_max_num_hi_scores(3);
  hi.store(13, "ccc");
  hi.store(11, "aaa");
  hi.store(12, "bbb");

  hi.store(10, "ddd");

  int score = -1;
  std::string name;
  hi.get_hi_score(0, score, name);
  REQUIRE(score == 11);
  REQUIRE(name == "aaa");
}


