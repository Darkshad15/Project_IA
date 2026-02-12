#include "ChaseState.h"
#include "NPC.h"
#include "Player.h"



void NpcAi::ChaseState::Enter(NpcContext& _context)
{
    std::cout << "Entering Chase State" << std::endl;
    _context.lostPlayerTimer = 0.f;
    _context.npc->SetSpriteState(SpriteState::WALK);

    ////  FORCER la sauvegarde de la position actuelle du joueur
    //if (_context.player != nullptr)
    //{
    //    _context.lastKnownPlayerPosition.x = _context.player->Getposition().x;
    //    _context.lastKnownPlayerPosition.y = _context.player->Getposition().y;

    //    std::cout << "CHASE ENTERED - Saved player position: ("
    //        << _context.lastKnownPlayerPosition.x << ", "
    //        << _context.lastKnownPlayerPosition.y << ")" << std::endl;
    //}
    //else
    //{
    //    std::cout << "ERROR: Player is nullptr when entering ChaseState!" << std::endl;
    //}
}
void ChaseState::Execute(NpcContext& context)
{
    if (context.player == nullptr)
    {
        std::cout << "ERROR: Player is nullptr in ChaseState!" << std::endl;
        return;
    }

    // DEBUG : Vérifier la vision
    bool canSeePlayer = Conditions::IsSeeingPlayer(context);

    //std::cout << "ChaseState Update - Can see player: " << (canSeePlayer ? "YES" : "NO") << std::endl;
    //std::cout << "  NPC pos: (" << context.position.x << ", " << context.position.y << ")" << std::endl;
    //std::cout << "  Player pos: (" << context.player->Getposition().x << ", " << context.player->Getposition().y << ")" << std::endl;
    //std::cout << "  NPC facing right: " << context.facingRight << std::endl;

    if (canSeePlayer)
    {
        // Mettre à jour la dernière position connue
        context.lastKnownPlayerPosition = context.player->Getposition();
        context.lostPlayerTimer = 0.f;

        //std::cout << "  -> Updated last known position to: ("
        //    << context.lastKnownPlayerPosition.x << ", "
        //    << context.lastKnownPlayerPosition.y << ")" << std::endl;

        // Se déplacer vers le joueur
        float dx = context.player->Getposition().x - context.position.x;
        float dy = context.player->Getposition().y - context.position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 1.0f)
        {
            context.velocity.x = (dx / distance) * 150.0f;
            context.velocity.y = (dy / distance) * 150.0f;
        }
        else
        {
            context.velocity.x = 0.0f;
            context.velocity.y = 0.0f;
        }
    }
    else
    {
        // On ne voit plus le joueur
        context.lostPlayerTimer += context.deltaTime;

        //std::cout << "  -> Lost player! Timer: " << context.lostPlayerTimer << std::endl;
        //std::cout << "  -> Going to last known: ("
        //    << context.lastKnownPlayerPosition.x << ", "
        //    << context.lastKnownPlayerPosition.y << ")" << std::endl;

        // Aller vers la dernière position connue
        float dx = context.lastKnownPlayerPosition.x - context.position.x;
        float dy = context.lastKnownPlayerPosition.y - context.position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 5.0f)
        {
            context.velocity.x = (dx / distance) * 100.0f;
            context.velocity.y = (dy / distance) * 100.0f;
        }
        else
        {
            context.velocity.x = 0.0f;
            context.velocity.y = 0.0f;
        }
    }

    // Mettre à jour la position
    context.position.x += context.velocity.x * context.deltaTime;
    context.position.y += context.velocity.y * context.deltaTime;
}

void NpcAi::ChaseState::Exit(NpcContext& _context)
{
    std::cout << "Exiting Chase State" << std::endl;
    _context.lostPlayerTimer = 0.f;  // Réinitialiser à la sortie
    _context.velocity.x = 0.0f;
    _context.velocity.y = 0.0f;
}