<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Relations\BelongsTo;
use Illuminate\Database\Eloquent\Model;

class UwbIps extends Model
{
    public function Entities(): BelongsTo
    {
        return $this->belongsTo(Entities::class, 'uwb', 'id');
    }
}
