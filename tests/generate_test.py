"File to generate test files for this project"

import argparse
import datetime
import dateutil.parser
from collections import defaultdict
import random

upperbound = 10000000

parser = argparse.ArgumentParser()
parser.add_argument('size', default=0, help='Number of elements in the Btree')
dynamic_parser = parser.add_mutually_exclusive_group(required=False)
dynamic_parser.add_argument('--dynamic', dest='dynamic', action='store_true')
dynamic_parser.add_argument('--static', dest='dynamic', action='store_false')
parser.set_defaults(dynamic=False)

args = parser.parse_args()
values = dict()


expected_input = list()
expected_output = list()


def generate_ISO_time(timestamp: int):
    return datetime.datetime.utcfromtimestamp(timestamp).replace(microsecond=0).isoformat()


def add_element(timestamp: int):
    timestamp_ISO = generate_ISO_time(timestamp)
    values[timestamp] = f"{timestamp}"
    expected_input.append(f"+{timestamp_ISO} {values[timestamp]}")


def delete_element(timestamp: int):
    timestamp_ISO = generate_ISO_time(timestamp)
    expected_input.append(f"-{timestamp_ISO}")
    if timestamp in values:
        del values[timestamp]
        return "-"
    else:
        return "?"


def search_element(timestamp: int):
    timestamp_ISO = generate_ISO_time(timestamp)
    expected_input.append(f"?{timestamp_ISO}")
    if timestamp in values:
        return f"!{values[timestamp]}"
    else:
        return "?"


def range_query(timestamp1: int):
    timestamp2 = timestamp1 + random.randint(0, upperbound)
    timestamp_ISO1 = generate_ISO_time(timestamp1)
    timestamp_ISO2 = generate_ISO_time(timestamp2)
    expected_input.append(f"#{timestamp_ISO1} {timestamp_ISO2}")
    return f"{len(list(filter(lambda i: timestamp1 <= i <= timestamp2, values.keys())))}"


for i in range(int(args.size)):
    if args.dynamic:
        random_fun = int(i//2 % 4)
    else:
        random_fun = int(i/(int(args.size)/4))
    retval = None
    timestamp = random.randint(0, upperbound)
    if random_fun == 0:
        add_element(timestamp)
    elif random_fun == 1:
        if random.random() < 0.5 and values.keys():
            timestamp = random.choice(list(values.keys()))
        retval = delete_element(timestamp)
    elif random_fun == 2:
        if random.random() < 0.95 and values.keys():
            timestamp = random.choice(list(values.keys()))
        retval = search_element(timestamp)
    else:
        retval = range_query(timestamp)

    if retval:
        expected_output.append(retval)

expected_output.append("")

if not args.dynamic:
    args.size = f"static.{args.size}"
with open(f"input.{args.size}", "w+") as f:
    f.write("\n".join(expected_input))
print("Written input file")

print("-----")

with open(f"output.{args.size}", "w+") as f:
    f.write("\n".join(expected_output))
print("Written output file")
