#include <stdio.h>
#include <string.h>
#include "hashmap.h"

struct user {
    char *name;
    int age;
};

int user_compare(const void *a, const void *b, void *udata) {
    const struct user *ua = a;
    const struct user *ub = b;
    return strcmp(ua->name, ub->name);
}

bool user_iter(const void *item, void *udata) {
    const struct user *user = item;
    printf("%s (age=%d)\n", user->name, user->age);
    return true;
}

uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct user *user = item;
    return 1;
}

int main() {
    // create a new hash map where each item is a `struct user`. The second
    // argument is the initial capacity. The third and fourth arguments are
    // optional seeds that are passed to the following hash function.
    struct hashmap *map = hashmap_new(sizeof(struct user), 0, 0, 0,
                                     user_hash, user_compare, NULL, NULL);

    // Here we'll load some users into the hash map. Each set operation
    // performs a copy of the data that is pointed to in the second argument.

    struct user in_user;

    in_user.name = "Thomas"; in_user.age=6;
    hashmap_set(map, &in_user);
    printf("set: name=%s, age=%d\n", in_user.name, in_user.age);

    in_user.name = "Roger"; in_user.age=5;
    hashmap_set(map, &in_user);
    printf("set: name=%s, age=%d\n", in_user.name, in_user.age);

    in_user.name = "Jane"; in_user.age=4;
    hashmap_set(map, &in_user);
    printf("set: name=%s, age=%d\n", in_user.name, in_user.age);

    struct user *rtn_thomas, *rtn_roger, *rtn_jane, *rtn_dale, *rtn_del_roger, *rtn_update_jane, *rtn_pat;

    printf("\n-- get some users --\n");

    in_user.name = "Thomas"; in_user.age=0;
    rtn_thomas = (struct user *)hashmap_get(map, &in_user);
    printf("rtn_thomas: %s age=%d (in: name=%s, age=%d)\n", rtn_thomas->name, rtn_thomas->age, in_user.name, in_user.age);

    in_user.name = "Roger"; in_user.age=0;
    rtn_roger = (struct user *)hashmap_get(map, &in_user);
    printf("rtn_roger: %s age=%d (in: name=%s, age=%d)\n", rtn_roger->name, rtn_roger->age, in_user.name, in_user.age);

    in_user.name = "Jane"; in_user.age=0;
    rtn_jane = (struct user *)hashmap_get(map, &in_user);
    printf("rtn_jane: %s age=%d (in: name=%s, age=%d)\n", rtn_jane->name, rtn_jane->age, in_user.name, in_user.age);

    in_user.name = "Dale"; in_user.age=0;
    rtn_dale = (struct user *)hashmap_get(map, &in_user);
    if (rtn_dale) {
      printf("BUG, found rtn_dale: %s age=%d (in: name=%s, age=%d)\n", rtn_dale->name, rtn_dale->age, in_user.name, in_user.age);
    } else {
      printf("not found (in: name=%s, age=%d)\n", in_user.name, in_user.age);
    }

    in_user.name = "Roger"; in_user.age=0;
    rtn_del_roger = hashmap_delete(map, &in_user);
    printf("delete roger; name=%s, age=%d\n", rtn_del_roger->name, rtn_del_roger->age);
    printf("rtn_thomas: name=%s, age=%d\n", rtn_thomas->name, rtn_thomas->age);
    printf("rtn_jane: name=%s, age=%d\n", rtn_jane->name, rtn_jane->age);
    printf("rtn_del_roger: name=%s, age=%d\n", rtn_del_roger->name, rtn_del_roger->age);

    in_user.name = "Jane"; in_user.age=44;
    rtn_update_jane = hashmap_set(map, &in_user);
    printf("Update Jane; old value: name=%s, age=%d\n", rtn_update_jane->name, rtn_update_jane->age);
    printf("rtn_thomas: name=%s, age=%d\n", rtn_thomas->name, rtn_thomas->age);
    printf("rtn_jane: name=%s, age=%d\n", rtn_jane->name, rtn_jane->age);
    printf("Whoa! pointer returned by initial set of Jane isn't updated!\n");
    printf("rtn_del_roger: name=%s, age=%d\n", rtn_del_roger->name, rtn_del_roger->age);
    printf("Whoa! pointer returned by deleting Roger now has Jane in it!\n");

    in_user.name = "Pat"; in_user.age=3;
    hashmap_set(map, &in_user);
    printf("set: name=%s, age=%d\n", in_user.name, in_user.age);
    printf("rtn_thomas: name=%s, age=%d\n", rtn_thomas->name, rtn_thomas->age);
    printf("rtn_jane: name=%s, age=%d\n", rtn_jane->name, rtn_jane->age);
    printf("Whoa! pointer returned by initial set of Jane now has Pat!\n");

    printf("\n-- iterate over all users (hashmap_scan) --\n");
    hashmap_scan(map, user_iter, NULL);

    printf("\n-- iterate over all users (hashmap_iter) --\n");
    size_t iter = 0;
    void *item;
    while (hashmap_iter(map, &iter, &item)) {
        const struct user *user = item;
        printf("%s (age=%d)\n", user->name, user->age);
    }

    hashmap_free(map);
}
