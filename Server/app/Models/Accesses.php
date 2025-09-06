<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Relations\BelongsTo;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Laravel\Sanctum\HasApiTokens;

class Accesses extends Authenticatable
{
    use HasApiTokens;

    protected $fillable = [
        'name',
        'secret',
    ];

    protected $hidden = [
        'secret',
    ];

    public function Entities(): BelongsTo
    {
        return $this->belongsTo(Entities::class, 'of_entity', 'id');
    }
}
