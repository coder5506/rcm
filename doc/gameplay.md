# Gameplay, or, The Theory and Practice of Reading Moves on a Centaur

## Theory

Earlier electronic chessboards used RFID to identify and locate the pieces on
the board.  The DGT Centaur innovates in this area by making the classic
tradeoff of simplifying the hardware by moving more of the work into software.
This has a number of advantages: it's cheaper, you can easily replace the
pieces, and you can repurpose the board for other games (e.g., checkers) or
even other non-game uses.

Instead of trying to recognize the pieces, the Centaur has simple sensors that
detect if a piece is present on a square or if a piece is removed from or added
to the board.  It's not perfect, it can sometimes fail to detect a smaller
piece (i.e., pawn) that is too far from the center of a square.  But it is
pretty good, you can slide a piece and it will detect all the transitions from
square to square.

The Centaur provides two complementary information streams that can be used to
interpret the user's actions as moves in a game.  First, you can always query
the board for the state of its fields: whether or not there is a piece present
on any given square.  And second, you can get a history of "actions", where
a piece is lifted from or placed onto a square.  The first stream is robust: if
you somehow fail to read the state of the board fields, you can just ask for
that again.  The second stream is less robust: if you fail to read a lift or a
place, it is gone, never to be seen again.

Chess being a rigid game, from any known position (this is important, and we'll
come back to it later), the state of the fields is sufficient to uniquely
identify most moves.  For example, in the starting position, if a piece
disappears from e2 and a piece appears on e4, that can mean only one thing,
and there's no need to worry about any lift or place events.  It doesn't matter
that the user first picked up the d-pawn, changed his mind, set it it back down,
and played the e-pawn.  We can ignore all of that.

We only need the actions (again, in a known position) to disambiguate some
captures.  If a piece or pawn can make two or more possible captures, these
captures are identical with respect to the resulting state of the sensors.  All
we see is that the capturing piece has "disappeared" from the board.  It has
replaced one of the other pieces, but we can't know which one without more
information.  For that, we need to know about the individual lift and place
events: we expect to see the capturing piece placed on the board in its
destination square.

In theory, that's all there is to it.

## Practice

The key to making the above work is that we read the state of the field sensors
from a known game position.  But we're polling the board at some fixed interval,
so it is always possible that multiple moves got played during that interval.
We did know the game position as of the last poll, but we may have missed one or
more intermediate positions, so that the state of the fields can no longer be
correctly interpreted with respect to our "known" position.

In theory, the board state alone is sufficient to determine most moves, and we
only need the tiniest bit of action history to decide between ambiguous
captures.

In practice, the actions become more important.  In the event that we missed one
or more moves, because play was too fast and multiple things happened during our
polling interval, we can use the action history to reconstruct intermediate
board states in order to find those missed moves.  We can replay each lift and
place to reconstruct the missed board states, then apply the "theory" algorithm
to reconstruct missed moves.

Missed moves should be rare.  A much more common problem is to read the field
sensors while a move is being made: a piece or two has been lifted, but nothing
has yet been placed.  Or in the case of en passant, the capturing pawn may have
been placed but the captured pawn has not yet been removed.

These transitional states are easy enough to detect.  The board state for a move
in progress can differ from the board state for a completed move in only a few
known squares.  When we identify such a state, we silently wait for the move
to be completed.  A move is only an error if it could not possibly be a
transition into any legal move.

# Implementation

When we put it all together, it looks like this:

1. Read the latest actions.  If there have been no actions, nothing is going on.
2. Read the current state of the fields.  If that state unambiguously matches a
   possible move, play it.  If it ambiguously matches more than one possible
   move, use the actions to disambiguate the move.
3. If we failed to match a move, possibly the board is in a transitional state.
   That is not an error and we'll pick up this move later.
4. But if we can't determine a move and we also can't justify the state as being
   in transition, well, it's probably an illegal move, but it is possible we
   missed a move somewhere.  Before crying foul, we'll replay the action history
   and look at intermediate board states.
5. Assuming we did not in fact miss a move in our polling, it's also possible
   the move was a takeback.  We can check for that by comparing the board state
   to previous positions
6. There is one edge case: if the user castled by moving the rook first, we
   may have previously recognized a legal rook move, now we revise it to
   castling.
7. If all that fails, we have to cry illegal move and wait for the board to
   be restored to some known state.  That could be any of: the last known
   position, a valid move from the last known position, some previous position
   (i.e., one or more takebacks), or the starting position (for a new game).
