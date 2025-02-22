/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../drawing/Drawing.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../../paint/tile_element/Paint.TileElement.h"
#include "../../sprites.h"
#include "../../world/Map.h"
#include "../RideData.h"
#include "../TrackData.h"
#include "../TrackPaint.h"

constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_0_B = 21658;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_1_A = 21647;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_1_B = 21659;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_2_A = 21648;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_0_A = 21646;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_2_B = 21660;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_3_A = 21649;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_DIR_3_B = 21661;

constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_0_A = 21638;
constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_0_B = 21650;
constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_1_A = 21639;
constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_1_B = 21651;
constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_2_A = 21640;
constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_2_B = 21652;
constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_3_A = 21641;
constexpr int32_t SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_3_B = 21653;

constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_0_A = 21642;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_0_B = 21654;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_1_A = 21643;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_1_B = 21655;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_2_A = 21644;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_2_B = 21656;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_3_A = 21645;
constexpr int32_t SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_3_B = 21657;

constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_0_A = 21882;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_0_B = 21886;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_1_A = 21883; // Needs no B piece
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_2_A = 21884;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_2_B = 21887;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_3_A = 21885; // Needs no B piece

// Whole block appears to lack B counterparts
constexpr int32_t SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_0_A = 21870;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_1_A = 21871;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_2_A = 21872;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_3_A = 21873;

constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_0_A = 21876;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_0_B = 21880;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_1_A = 21877; // Needs no B piece
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_2_A = 21878;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_2_B = 21881;
constexpr int32_t SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_3_A = 21879; // Needs no B piece

/** rct2: 0x0077839C */
static void side_friction_rc_track_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackElement.HasChain())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21662, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21664, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
                wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21663, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21665, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
                wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21666, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21668, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
                wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21667, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21669, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
                wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    else
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21606, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21608, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
                wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21607, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21609, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
                wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x007784AC, 0x007784BC, 0x007784CC */
static void side_friction_rc_track_station(
    paint_session& session, const Ride& ride, [[maybe_unused]] uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    static constexpr const uint32_t imageIds[4] = {
        21610,
        21611,
        21610,
        21611,
    };

    PaintAddImageAsParentRotated(
        session, direction, imageIds[direction] | session.TrackColours[SCHEME_TRACK], 0, 0, 32, 27, 2, height, 0, 2, height);
    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
    track_paint_util_draw_station_2(session, ride, direction, height, trackElement, 9, 11);
    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x007783AC */
static void side_friction_rc_track_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackElement.HasChain())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21678, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21690, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 9, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21679, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21691, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 10, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21680, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21692, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 11, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21681, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21693, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 12, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21622, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21634, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 9, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21623, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21635, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 10, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21624, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21636, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 11, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21625, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21637, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 12, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_8);
    }
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 56, 0x20);
}

/** rct2: 0x007783CC */
static void side_friction_rc_track_flat_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackElement.HasChain())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21670, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21682, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21671, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21683, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 2, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21672, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21684, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 3, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21673, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21685, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21614, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21626, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21615, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21627, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 2, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21616, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21628, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 3, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21617, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21629, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_8);
    }
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 48, 0x20);
}

/** rct2: 0x007783FC */
static void side_friction_rc_track_25_deg_up_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackElement.HasChain())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21674, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21686, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 5, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21675, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21687, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21676, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21688, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 7, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21677, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21689, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21618, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21630, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 5, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21619, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21631, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21620, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21632, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 0, 7, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21621, 0, 0, 32, 27, 2, height, 0, 2, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 21633, 0, 0, 32, 1, 9, height, 0, 26, height + 5);
                wooden_a_supports_paint_setup(session, 1, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                break;
        }
    }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_FLAT);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
    }
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 40, 0x20);
}

/** rct2: 0x0077840C */
static void side_friction_rc_track_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_25_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x0077842C */
static void side_friction_rc_track_flat_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_25_deg_up_to_flat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x0077845C */
static void side_friction_rc_track_25_deg_down_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_flat_to_25_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x0077846C */
static void side_friction_rc_track_left_quarter_turn_5(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21727, 0, 2, 32, 27, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21747, 0, 2, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21732, 0, 2, 32, 27, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21752, 0, 2, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21737, 0, 2, 32, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21757, 0, 2, 32, 32, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21722, 0, 2, 32, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21742, 0, 2, 32, 32, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21726, 0, 0, 32, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21746, 0, 0, 32, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21731, 0, 0, 32, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21751, 0, 0, 32, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21736, 0, 16, 32, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21756, 0, 16, 32, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21721, 0, 16, 32, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21741, 0, 16, 32, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21725, 0, 16, 16, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21745, 0, 16, 16, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21730, 16, 16, 16, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21750, 16, 16, 16, 16, 0, height, 16, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21735, 16, 0, 16, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21755, 16, 0, 16, 16, 0, height, 16, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21720, 0, 0, 16, 16, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21740, 0, 0, 16, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4,
                    direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 4:
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 5:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21724, 16, 0, 16, 34, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21744, 16, 0, 16, 34, 0, height, 16, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21729, 0, 0, 16, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21749, 0, 0, 16, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21734, 0, 0, 16, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21754, 0, 0, 16, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21719, 16, 0, 16, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21739, 16, 0, 16, 32, 0, height, 16, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 6:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21723, 2, 0, 32, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21743, 2, 0, 32, 32, 0, height, 2, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21728, 2, 0, 27, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21748, 2, 0, 27, 32, 0, height, 2, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21733, 2, 0, 27, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21753, 2, 0, 27, 32, 0, height, 2, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21718, 2, 0, 32, 32, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21738, 2, 0, 32, 32, 0, height, 2, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            switch (direction)
            {
                case 2:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 3:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x0077847C */
static void side_friction_rc_track_right_quarter_turn_5(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
    side_friction_rc_track_left_quarter_turn_5(session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x0077848C */
static void side_friction_rc_track_s_bend_left(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21888, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21904, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21892, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21908, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21891, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21907, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21895, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21911, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21889, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21905, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21893, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21909, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21890, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21906, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21894, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21910, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21890, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21906, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21894, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21910, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21889, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21905, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21893, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21909, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21891, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21907, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21895, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21911, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21888, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21904, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21892, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21908, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            switch (direction)
            {
                case 1:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 2:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x0077849C */
static void side_friction_rc_track_s_bend_right(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21896, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21912, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21900, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21916, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21899, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21915, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21903, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21919, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21897, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21913, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21901, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21917, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21898, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21914, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21902, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21918, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21898, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21914, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21902, 0, 0, 32, 26, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21918, 0, 0, 32, 26, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21897, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21913, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21901, 0, 0, 32, 26, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21917, 0, 0, 32, 26, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21899, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21915, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21903, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21919, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21896, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21912, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21900, 0, 0, 32, 27, 2, height, 0, 2, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21916, 0, 0, 32, 27, 0, height, 0, 2,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            switch (direction)
            {
                case 1:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 2:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x007784DC */
static void side_friction_rc_track_left_quarter_turn_3(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21699, 0, 0, 32, 20, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21711, 0, 0, 32, 20, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21702, 0, 0, 32, 20, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21714, 0, 0, 32, 20, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21705, 0, 0, 32, 20, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21717, 0, 0, 32, 20, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21696, 0, 0, 32, 20, 2, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21708, 0, 0, 32, 20, 0, height, 0, 6,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21698, 0, 0, 16, 16, 2, height, 16, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21710, 0, 0, 16, 16, 0, height, 16, 0,
                        height + 27);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21701, 0, 0, 16, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21713, 0, 0, 16, 16, 0, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21704, 0, 0, 16, 16, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21716, 0, 0, 16, 16, 0, height, 0, 16,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21695, 0, 0, 16, 16, 2, height, 16, 16,
                        height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21707, 0, 0, 16, 16, 0, height, 16, 16,
                        height + 27);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21697, 0, 0, 20, 32, 2, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21709, 0, 0, 20, 32, 0, height, 6, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21700, 0, 0, 20, 32, 2, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21712, 0, 0, 20, 32, 0, height, 6, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21703, 0, 0, 20, 32, 2, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21715, 0, 0, 20, 32, 0, height, 6, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21694, 0, 0, 20, 32, 2, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21706, 0, 0, 20, 32, 0, height, 6, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            switch (direction)
            {
                case 2:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 3:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x007784EC */
static void side_friction_rc_track_right_quarter_turn_3(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    side_friction_rc_track_left_quarter_turn_3(session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x007784FC */
static void side_friction_rc_track_brakes(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (direction)
    {
        case 0:
        case 2:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | 21610, 0, 0, 32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | 21612, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
            wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
        case 3:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | 21611, 0, 0, 32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | 21613, 0, 0, 32, 27, 0, height, 0, 2, height + 27);
            wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
    }
    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x007785DC */
static void side_friction_rc_track_left_eighth_to_diag(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21790, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21806, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21794, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21810, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21798, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21814, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21802, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21818, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21791, 0, 0, 32, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21807, 0, 0, 32, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21795, 0, 0, 34, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21811, 0, 0, 32, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21799, 0, 0, 32, 16, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21815, 0, 0, 32, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21803, 0, 0, 32, 16, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21819, 0, 0, 32, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21792, 0, 0, 16, 16, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21808, 0, 0, 16, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21796, 0, 0, 16, 16, 2, height, 16, 16,
                        height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21812, 0, 0, 16, 16, 0, height, 16, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21800, 0, 0, 16, 16, 2, height, 16, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21816, 0, 0, 16, 16, 0, height, 16, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21804, 0, 0, 16, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21820, 0, 0, 16, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 4:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21793, 0, 0, 16, 16, 2, height, 16, 16,
                        height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21809, 0, 0, 16, 16, 0, height, 16, 16,
                        height + 27);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21797, 0, 0, 16, 18, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21813, 0, 0, 16, 16, 0, height, 0, 16,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21801, 0, 0, 16, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21817, 0, 0, 16, 16, 0, height, 0, 0,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21805, 0, 0, 16, 16, 2, height, 16, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21821, 0, 0, 16, 16, 0, height, 16, 0,
                        height + 27);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x007785EC */
static void side_friction_rc_track_right_eighth_to_diag(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21758, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21774, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21762, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21778, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21766, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21782, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21770, 0, 0, 32, 32, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21786, 0, 0, 32, 32, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21759, 0, 0, 32, 16, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21775, 0, 0, 32, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21763, 0, 0, 32, 16, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21779, 0, 0, 32, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21767, 0, 0, 34, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21783, 0, 0, 32, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21771, 0, 0, 32, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21787, 0, 0, 32, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21760, 0, 0, 16, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21776, 0, 0, 16, 16, 0, height, 0, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21764, 0, 0, 16, 16, 2, height, 16, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21780, 0, 0, 16, 16, 0, height, 16, 0,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21768, 0, 0, 28, 28, 2, height, 4, 4, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21784, 0, 0, 28, 28, 0, height, 4, 4,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21772, 0, 0, 16, 16, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21788, 0, 0, 16, 16, 0, height, 0, 16,
                        height + 27);
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 4:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21761, 0, 0, 16, 16, 2, height, 16, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21777, 0, 0, 16, 16, 0, height, 16, 0,
                        height + 27);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21765, 0, 0, 16, 16, 2, height, 0, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21781, 0, 0, 16, 16, 0, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21769, 0, 0, 16, 18, 2, height, 0, 16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21785, 0, 0, 16, 16, 0, height, 0, 16,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21773, 0, 0, 16, 16, 2, height, 16, 16,
                        height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 21789, 0, 0, 16, 16, 0, height, 16, 16,
                        height + 27);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x007785FC */
static void side_friction_rc_track_left_eighth_to_orthogonal(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    side_friction_rc_track_right_eighth_to_diag(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x0077860C */
static void side_friction_rc_track_right_eighth_to_orthogonal(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    side_friction_rc_track_left_eighth_to_diag(session, ride, trackSequence, (direction + 3) & 3, height, trackElement);
}

/** rct2: 0x0077850C */
static void side_friction_rc_track_diag_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21849, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21825, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21846, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21850, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21822, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21826, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21848, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21851, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21824, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21827, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21847, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21823, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x0077853C */
static void side_friction_rc_track_diag_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21867, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21843, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 1:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21864, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21868, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21840, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21844, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 2:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21866, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21869, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21842, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21845, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 3:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21865, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21841, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
    }
}

/** rct2: 0x0077851C */
static void side_friction_rc_track_diag_flat_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21855, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21831, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
        case 1:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21852, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21856, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21828, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21832, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
        case 2:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21854, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21857, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21830, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21833, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
        case 3:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21853, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21829, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
    }
}

/** rct2: 0x0077852C */
static void side_friction_rc_track_diag_25_deg_up_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21861, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21837, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 1:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21858, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21862, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21834, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21838, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 2:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21860, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21863, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21836, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21839, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 3:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21859, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21835, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
    }
}

/** rct2: 0x0077859C */
static void side_friction_rc_track_diag_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21865, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21841, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 1:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21866, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21869, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21842, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21845, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 2:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21864, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21868, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21840, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21844, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 43);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
        case 3:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21867, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21843, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 56, 0x20);
            break;
    }
}

/** rct2: 0x0077857C */
static void side_friction_rc_track_diag_flat_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21859, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21835, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            break;
        case 1:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21860, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21863, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21836, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21839, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            break;
        case 2:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21858, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21862, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21834, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21838, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            break;
        case 3:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21861, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21837, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            break;
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 56, 0x20);
}

/** rct2: 0x0077858C */
static void side_friction_rc_track_diag_25_deg_down_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21853, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21829, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
        case 1:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21854, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21857, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21830, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21833, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
        case 2:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 0:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21852, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21856, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 0:
                        wooden_a_supports_paint_setup(session, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        wooden_a_supports_paint_setup(session, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21828, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21832, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        wooden_a_supports_paint_setup(session, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        wooden_a_supports_paint_setup(session, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
        case 3:
            if (trackElement.HasChain())
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21855, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            else
            {
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 21831, -16, -16, 32, 32, 2, height, -16,
                            -16, height);
                        break;
                }
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 48, 0x20);
            break;
    }
}

static void side_friction_rc_track_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (direction)
    {
        case 0:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_0_A, 0, 0, 32, 27, 2,
                height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_0_B, 0, 0, 32, 1, 9,
                height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 0, 21, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_1_A, 0, 0, 32, 27, 2,
                height, 0, 2, height);

            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_1_B, 0, 0, 32, 1, 9,
                height, 0, 26, height + 5);

            wooden_a_supports_paint_setup(session, 1, 22, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_2_A, 0, 0, 32, 27, 2,
                height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_2_B, 0, 0, 32, 1, 9,
                height, 0, 26, height + 5);

            wooden_a_supports_paint_setup(session, 0, 23, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_3_A, 0, 0, 32, 27, 2,
                height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_DIR_3_B, 0, 0, 32, 1, 9,
                height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 1, 24, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
    }

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_8);
    }
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 104, 0x20);
}

static void side_friction_rc_track_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_60_deg_up(session, ride, trackSequence, (direction + 2) % 4, height, trackElement);
}

static void side_friction_rc_track_25_deg_up_to_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (direction)
    {
        case 0:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_0_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_0_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 0, 13, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_1_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_1_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 1, 14, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_2_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_2_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 0, 15, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_3_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_25_DEG_UP_TO_60_DEG_UP_DIR_3_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 1, 16, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
    }

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_8);
    }
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void side_friction_rc_track_60_deg_down_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_25_deg_up_to_60_deg_up(session, ride, trackSequence, (direction + 2) % 4, height, trackElement);
}

static void side_friction_rc_track_60_deg_up_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (direction)
    {
        case 0:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_0_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_0_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 0, 17, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_1_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_1_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 1, 18, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_2_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_2_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 0, 19, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_3_A, 0, 0,
                32, 27, 2, height, 0, 2, height);
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_60_DEG_UP_TO_25_DEG_UP_DIR_3_B, 0, 0,
                32, 1, 9, height, 0, 26, height + 5);
            wooden_a_supports_paint_setup(session, 1, 20, height, session.TrackColours[SCHEME_SUPPORTS]);
            break;
    }

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_8);
    }
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void side_friction_rc_track_25_deg_down_to_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_60_deg_up_to_25_deg_up(session, ride, trackSequence, (direction + 2) % 4, height, trackElement);
}

static void side_friction_rc_track_diag_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_3_A, -16,
                        -16, 32, 32, 2, height, -16, -16, height);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 104, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_0_A, -16,
                        -16, 32, 32, 2, height, -16, -16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_0_B, -16,
                        -16, 32, 32, 0, height, -16, -16, height + 43);
                    wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }

            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 104, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_2_A, -16,
                        -16, 32, 32, 2, height, -16, -16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_2_B, -16,
                        -16, 32, 32, 0, height, -16, -16, height + 43);
                    wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 104, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_DIR_1_A, -16,
                        -16, 32, 32, 2, height, -16, -16, height);
                    break;
            }

            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 104, 0x20);
            break;
    }
}

static void side_friction_rc_track_diag_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_diag_60_deg_up(session, ride, 3 - trackSequence, (direction + 2) % 4, height, trackElement);
}

static void side_friction_rc_track_diag_60_deg_up_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_3_A, -16, -16,
                        32, 32, 2, height, -16, -16, height);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_0_A, -16, -16,
                        32, 32, 2, height, -16, -16, height);
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_0_B, -16, -16,
                        32, 32, 0, height, -16, -16, height + 59);
                    wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_2_A, -16, -16,
                        32, 32, 2, height, -16, -16, height);
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_2_B, -16, -16,
                        32, 32, 0, height, -16, -16, height + 59);
                    wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_60_DEG_UP_TO_25_DEG_UP_DIR_1_A, -16, -16,
                        16, 16, 2, height, 0, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

static void side_friction_rc_track_diag_25_deg_down_to_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_diag_60_deg_up_to_25_deg_up(
        session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement);
};

static void side_friction_rc_track_diag_25_deg_up_to_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_3_A, -16 + 4,
                        -16 + 2, 32, 32, 2, height, -16, -16, height);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_0_A, -16, -16,
                        32, 32, 2, height, -16, -16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 24033, -16, -16, 32, 32, 0, height, -16, -16,
                        height + 59);
                    wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    wooden_b_supports_paint_setup(session, 4, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    wooden_b_supports_paint_setup(session, 5, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_2_A, -16, -16,
                        32, 32, 2, height, -16, -16, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 24034, -16, -16, 32, 32, 0, height, -16, -16,
                        height + 59);
                    wooden_b_supports_paint_setup(session, 2, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    wooden_b_supports_paint_setup(session, 3, 0, height + 16, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours[SCHEME_TRACK] | SPR_SIDE_FRICTION_DIAG_25_DEG_UP_TO_60_DEG_UP_DIR_1_A, -16, -16,
                        32, 32, 2, height, -16, -16, height);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

static void side_friction_rc_track_diag_60_deg_down_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    side_friction_rc_track_diag_25_deg_up_to_60_deg_up(
        session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement);
};

TRACK_PAINT_FUNCTION get_track_paint_function_side_friction_rc(int32_t trackType)
{
    switch (trackType)
    {
        case TrackElemType::Flat:
            return side_friction_rc_track_flat;
        case TrackElemType::EndStation:
        case TrackElemType::BeginStation:
        case TrackElemType::MiddleStation:
            return side_friction_rc_track_station;
        case TrackElemType::Up25:
            return side_friction_rc_track_25_deg_up;
        case TrackElemType::FlatToUp25:
            return side_friction_rc_track_flat_to_25_deg_up;
        case TrackElemType::Up25ToFlat:
            return side_friction_rc_track_25_deg_up_to_flat;
        case TrackElemType::Down25:
            return side_friction_rc_track_25_deg_down;
        case TrackElemType::FlatToDown25:
            return side_friction_rc_track_flat_to_25_deg_down;
        case TrackElemType::Down25ToFlat:
            return side_friction_rc_track_25_deg_down_to_flat;
        case TrackElemType::LeftQuarterTurn5Tiles:
            return side_friction_rc_track_left_quarter_turn_5;
        case TrackElemType::RightQuarterTurn5Tiles:
            return side_friction_rc_track_right_quarter_turn_5;
        case TrackElemType::SBendLeft:
            return side_friction_rc_track_s_bend_left;
        case TrackElemType::SBendRight:
            return side_friction_rc_track_s_bend_right;
        case TrackElemType::LeftQuarterTurn3Tiles:
            return side_friction_rc_track_left_quarter_turn_3;
        case TrackElemType::RightQuarterTurn3Tiles:
            return side_friction_rc_track_right_quarter_turn_3;
        case TrackElemType::Brakes:
            return side_friction_rc_track_brakes;
        case TrackElemType::LeftEighthToDiag:
            return side_friction_rc_track_left_eighth_to_diag;
        case TrackElemType::RightEighthToDiag:
            return side_friction_rc_track_right_eighth_to_diag;
        case TrackElemType::LeftEighthToOrthogonal:
            return side_friction_rc_track_left_eighth_to_orthogonal;
        case TrackElemType::RightEighthToOrthogonal:
            return side_friction_rc_track_right_eighth_to_orthogonal;
        case TrackElemType::DiagFlat:
            return side_friction_rc_track_diag_flat;
        case TrackElemType::DiagUp25:
            return side_friction_rc_track_diag_25_deg_up;
        case TrackElemType::DiagFlatToUp25:
            return side_friction_rc_track_diag_flat_to_25_deg_up;
        case TrackElemType::DiagUp25ToFlat:
            return side_friction_rc_track_diag_25_deg_up_to_flat;
        case TrackElemType::DiagDown25:
            return side_friction_rc_track_diag_25_deg_down;
        case TrackElemType::DiagFlatToDown25:
            return side_friction_rc_track_diag_flat_to_25_deg_down;
        case TrackElemType::DiagDown25ToFlat:
            return side_friction_rc_track_diag_25_deg_down_to_flat;

        // Added by OpenRCT2
        case TrackElemType::Down25ToDown60:
            return side_friction_rc_track_25_deg_down_to_60_deg_down;
        case TrackElemType::Down60ToDown25:
            return side_friction_rc_track_60_deg_down_to_25_deg_down;
        case TrackElemType::Up25ToUp60:
            return side_friction_rc_track_25_deg_up_to_60_deg_up;
        case TrackElemType::Up60ToUp25:
            return side_friction_rc_track_60_deg_up_to_25_deg_up;
        case TrackElemType::Up60:
            return side_friction_rc_track_60_deg_up;
        case TrackElemType::Down60:
            return side_friction_rc_track_60_deg_down;

        case TrackElemType::DiagUp60:
            return side_friction_rc_track_diag_60_deg_up;
        case TrackElemType::DiagDown60:
            return side_friction_rc_track_diag_60_deg_down;
        case TrackElemType::DiagUp60ToUp25:
            return side_friction_rc_track_diag_60_deg_up_to_25_deg_up;
        case TrackElemType::DiagDown25ToDown60:
            return side_friction_rc_track_diag_25_deg_down_to_60_deg_down;
        case TrackElemType::DiagUp25ToUp60:
            return side_friction_rc_track_diag_25_deg_up_to_60_deg_up;
        case TrackElemType::DiagDown60ToDown25:
            return side_friction_rc_track_diag_60_deg_down_to_25_deg_down;
    }
    return nullptr;
}
