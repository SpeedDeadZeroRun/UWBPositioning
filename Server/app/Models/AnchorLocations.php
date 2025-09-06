<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Relations\BelongsTo;
use Illuminate\Database\Eloquent\Model;

class AnchorLocations extends Model
{
    public function Entities(): BelongsTo
    {
        return $this->belongsTo(Entities::class, 'anchor', 'id');
    }
}
