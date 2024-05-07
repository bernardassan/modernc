#include <assert.h>
#include <stdbit.h>
#include <stdio.h>

typedef struct birds {
  char const *jay;
  char const *magpie;
  char const *raven;
  char const *chough;
} birds;

static birds const bird [[maybe_unused]] = {
    .jay = "jay",
    .magpie = "magpie",
    .raven = "raven",
    .chough = "chough",
};

enum corvid : uint8_t {
  chough,
  magpie,
  raven,
  jay,
  corvid_num,
};

enum vehicle { bus, airplane, train, car, horse, feet };

static enum vehicle choose(char arg) {
  switch (arg) {
  case 'B':
    return bus;
  case 'A':
    return airplane;
  case 'T':
    return train;
  case 'C':
    return car;
  case 'H':
    return horse;
  default:
    return feet;
  }
}

[[maybe_unused]] static void use_choose() {
  assert(choose('H') == horse && choose('F') == feet);
}

constexpr static char CORVID_NAME[corvid_num][8] = {
    [chough] = "chough",
    [raven] = "raven",
    [magpie] = "magpie",
    [jay] = "jay",
};

static void enumerations() {

#define FLOCK_MAGPIE (1U << magpie)
#define FLOCK_RAVEN (1U << raven)
#define FLOCK_JAY (1U << jay)
#define FLOCK_CHOUGH (1U << chough)
#define FLOCK_EMPTY 0U
#define FLOCK_FULL ((1U << corvid_num) - 1)

  for (size_t index = 0; index < corvid_num; ++index) {
    printf("Corvid %zu is the %s\n", index, CORVID_NAME[index]);
  }
}
typedef enum Weekends {
  Friday,
  Saturday,
  Sunday,
} Weekends;

[[maybe_unused]] static void switching(Weekends const day) {

  [[maybe_unused]] Weekends bestday = Friday;
  [[maybe_unused]] Weekends secondbest = Saturday;

  Weekends churchday = Sunday;

  bestday = churchday;
  secondbest = bestday;
  bool boolean = true;

  switch (day) {
  case Friday:
    printf("\nIt is friday\n");
    if (boolean && (churchday == Friday)) {
      boolean = false;
      churchday = Friday;
    }
    break;
  case Saturday:
    printf("\nIts Saturday\n");
    return;
  case Sunday:
    printf("\nIts sunday,go to church\n");
    return;
  }
}
