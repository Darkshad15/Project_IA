#include "ChaseState.h"
#include "NPC.h"
#include "Player.h"



void NpcAi::ChaseState::Enter(NpcContext& _context)
{
    std::cout << "Entering Chase State" << std::endl;
    _context.lostPlayerTimer = 0.f;  //Réinitialiser le timer
    _context.npc->SetSpriteState(SpriteState::WALK);
}

void NpcAi::ChaseState::Execute(NpcContext& _context)
{
    if (_context.player == nullptr)
    {
        return;
    }

    // Vérifier si on voit toujours le joueur
    if (Conditions::IsSeeingPlayer(_context))
    {
        // On voit le joueur : réinitialiser le timer
        _context.lostPlayerTimer = 0.f;
        _context.lastKnownPlayerPosition = _context.player->Getposition();

        // Se déplacer vers le joueur
        float dx = _context.player->Getposition().x - _context.position.x;
        float dy = _context.player->Getposition().y - _context.position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 1.0f)
        {
            _context.velocity.x = (dx / distance) * 150.0f;  // Vitesse de poursuite
            _context.velocity.y = (dy / distance) * 150.0f;
        }
        else
        {
            _context.velocity.x = 0.0f;
            _context.velocity.y = 0.0f;
        }
    }
    else
    {
        // On ne voit plus le joueur : incrémenter le timer
        _context.lostPlayerTimer += _context.deltaTime;  // Vous devez passer deltaTime

        // Optionnel : aller vers la dernière position connue
        float dx = _context.lastKnownPlayerPosition.x - _context.position.x;
        float dy = _context.lastKnownPlayerPosition.y - _context.position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 5.0f)
        {
            _context.velocity.x = (dx / distance) * 100.0f;
            _context.velocity.y = (dy / distance) * 100.0f;
        }
        else
        {
            // Arrivé à la dernière position connue
            _context.velocity.x = 0.0f;
            _context.velocity.y = 0.0f;
        }
    }

    // Mettre à jour la position
    _context.position.x += _context.velocity.x * _context.deltaTime;
    _context.position.y += _context.velocity.y * _context.deltaTime;
}

void NpcAi::ChaseState::Exit(NpcContext& _context)
{
    std::cout << "Exiting Chase State" << std::endl;
    _context.lostPlayerTimer = 0.f;  // Réinitialiser à la sortie
    _context.velocity.x = 0.0f;
    _context.velocity.y = 0.0f;
}