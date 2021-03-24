#include "G2_G3_goal.h"

bool G2_G3_Goal::contented(const Rubiks& cube) const
{
    using ECOLOUR = Rubiks::ECOLOUR;
    using EPIECE  = Rubiks::EPIECE;
    using EEDGE   = Rubiks::EEDGE;

    using pair_t  = std::array<uint8_t, 2>;

    // stores which corner is currently occupying which position
    std::array<uint8_t, 8> cPosPerm = {
        cube.getPieceInd(EPIECE::ULB),
        cube.getPieceInd(EPIECE::ULF),
        cube.getPieceInd(EPIECE::DLF),
        cube.getPieceInd(EPIECE::DLB),
        cube.getPieceInd(EPIECE::DRB),
        cube.getPieceInd(EPIECE::DRF),
        cube.getPieceInd(EPIECE::URF),
        cube.getPieceInd(EPIECE::URB),
    };

    // finds the positions of a pair and writes it to result
    auto setPairPos = [&](const pair_t& target, pair_t& result) {
        for (uint8_t i = 0; i < 8; ++i)
        {
            if (target[0] == cPosPerm[i]) result[0] = i;
            if (target[1] == cPosPerm[i]) result[1] = i;
        }
    };

    // root (solved) pair positions. for a solved state a corner index matches the position index in cPosPerm
    constexpr std::array<pair_t, 4> rootPairs = {{ {0, 2}, {4, 6}, {1, 3}, {5, 7} }};

    // stores the positions of paired corners
    std::array<pair_t, 4> cPairPerm;
    for (uint8_t i = 0; i < 4; ++i)
    {
        setPairPos(rootPairs[i], cPairPerm[i]);
    }
    // checks if the pairs are formed
    for (auto& pair : cPairPerm)
    {
        if ((pair[0] & 5) != (pair[1] & 5))
        {
            return false;
        }
    }

    uint8_t parity = 0;

    // checks if parity is even or odd
    for (uint8_t i = 0; i < 8; ++i)
    {
        for (uint8_t j = i + 1; j < 8; ++j)
        {
            parity ^= cPosPerm[i] < cPosPerm[j];
        }
    }
    // returns false if parity is odd
    if (parity == 1)
    {
        return false;
    }

    // checks if all the edges are in their home slice (M-slice edges are already solved)
    return
        (cube.getColour(EEDGE::UR) == ECOLOUR::W || cube.getColour(EEDGE::UR) == ECOLOUR::Y) &&
        (cube.getColour(EEDGE::UL) == ECOLOUR::W || cube.getColour(EEDGE::UL) == ECOLOUR::Y) &&
        (cube.getColour(EEDGE::DR) == ECOLOUR::W || cube.getColour(EEDGE::DR) == ECOLOUR::Y) &&
        (cube.getColour(EEDGE::DL) == ECOLOUR::W || cube.getColour(EEDGE::DL) == ECOLOUR::Y) &&

        (cube.getColour(EEDGE::FR) == ECOLOUR::G || cube.getColour(EEDGE::FR) == ECOLOUR::B) &&
        (cube.getColour(EEDGE::FL) == ECOLOUR::G || cube.getColour(EEDGE::FL) == ECOLOUR::B) &&
        (cube.getColour(EEDGE::BR) == ECOLOUR::G || cube.getColour(EEDGE::BR) == ECOLOUR::B) &&
        (cube.getColour(EEDGE::BL) == ECOLOUR::G || cube.getColour(EEDGE::BL) == ECOLOUR::B);
}
