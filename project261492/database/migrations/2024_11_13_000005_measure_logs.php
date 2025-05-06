<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('measure_logs', function (Blueprint $table) {
            $table->id()->primary();
            $table->timestamp('timestamp');
            $table->foreignId('anchor')->references('id')->on('entities');
            $table->foreignId('tag')->references('id')->on('entities');
            $table->float('distance', precision: 24);
            $table->float('rssi', precision: 24);
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('measure_logs');
    }
};
