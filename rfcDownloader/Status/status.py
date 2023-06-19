from enum import Enum

class Status(Enum):
    OK = 200
    BAD_REQUEST = 400
    NOT_FOUND = 404
    P2P_CI_NOT_SUPPORTED = 505